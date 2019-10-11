#ifndef ROBOT_H
#define ROBOT_H
#pragma once

// mathematical constants to help with rotating the robot in radians e.g. M_PI
#define _USE_MATH_DEFINES

#include <libplayerc++/playerc++.h>
#include <cmath>
#include "Vector2.h"

/**
 * Enum used to represent a direction for the robot to turn.
 * This is set as a namespace to avoid potential naming conflicts.
 * A modern day C++11 solution would be to use an enum class but,
 * to avoid any sort of dependency error, this namespace approach
 * is being taken instead.
 */
namespace TurnDirection
{
  enum Enum { Left, Right, Random, None };
}

/**
 * Struct utilized to define the direction the robot should rotate
 * depending on which bumper was pressed
 */ 
struct HandleBumpConfig
{
  TurnDirection::Enum both,  // direction to turn if both bumpers hit
                      left,  // direction to turn if left bumper hit
                      right; // direction to turn if right bumper hit

  /** Constructs a new HandleBumpConfig object */
  HandleBumpConfig(TurnDirection::Enum both  = TurnDirection::Random,
                   TurnDirection::Enum left  = TurnDirection::Right,
                   TurnDirection::Enum right = TurnDirection::Left) : both(both), left(left), right(right) {};
};

/**
 * Wrapper class used to simplify use of the Robot
 */ 
class Robot
{
  PlayerCc::PlayerClient    robot;
  PlayerCc::Position2dProxy pp;         // The 2D proxy is used to send motion commands.
  PlayerCc::BumperProxy     bp;         // The bumper proxy reads from the bumpers.
  PlayerCc::LocalizeProxy   lp;         // Used to control a localize device for localization
  PlayerCc::LaserProxy     *sp;         // Laser proxy used to scan the environment
  bool isHandlingBump;                  // true if the robot is currently correcting its position due to a bumper press

  // tick interval of the robot
  const double TICK_INTERVAL;

  // scale movement and rotation of the robot to ensure accurate locomotion
  const double MOVEMENT_SCALE,
               ROTATION_SCALE;

  // movement
  void moveAndRotateOverTicks(double forwardVelocity, double angularVelocity, int ticks);
  void getFinalTicksAndVelocity(double distance, double& velocity, int& ticks);

  // waypoint movement
  void getAngleDistanceToWaypoint(Vector2& pos, double yaw, Vector2& wp, double& angle, double& distance);

public:
  // constructor
  Robot(bool   isUsingLaser  = true,
        double movementScale = 1.0,
        double rotationScale = 1.0,
        double tickInterval  = 0.1,
        std::string hostname = "localhost");

  // destructor
  ~Robot();

  // read from the environment
  void read();

  // get position based on odometer
  double getOdometerYaw();
  Vector2 getOdometerPos();

  // get position based on localization
  Vector2 getLocalizedPos();
  double getLocalizedYaw();

  // get status of bumpers
  bool isLeftPressed();
  bool isRightPressed();
  bool isAnyPressed();

  // print information about the robot
  void printOdometerPosition();
  void printLocalizedPosition();
  void printAllHypotheses();
  void printBumper();
  void printLaserData();
  
  // get the best Hypothesis from the LocalizeProxy 
  player_localize_hypoth_t getBestLocalizeHypothesis();

  // get the best pose from the LocalizeProxy
  player_pose2d_t getPoseFromLocalizeProxy();

  // localizes the robot
  void localize();

  // motor
  void setMotorEnable(bool isMotorEnabled);

  // handle basic movement
  void moveForwardByMeters(double distanceInMeters, double forwardVelocity = 0.5);
  void rotateByRadians(double radiansToRotate, double angularVelocity = 0.5);

  // wrapper method for pp.SetSpeed()
  void setSpeed(double forwardVelocity = 0.5,
                double angularVelocity = 0.5,
                TurnDirection::Enum dir = TurnDirection::Left);

  // bumper movement
  void handleBump(HandleBumpConfig bumpConfig = HandleBumpConfig(),
                  double angle           = 5.0 * M_PI / 12.0,  // ~75 degrees
                  double distance        = 1.0,
                  double velocity        = 1.0,
                  double angularVelocity = 0.5);

  // handle waypoint movement
  bool hasReachedWaypoint(Vector2& pos, Vector2& wp, double errorRange);
  void moveToWaypoint(Vector2& wp,
                      bool useLocalization   = false,
                      double velocity        = 0.5,
                      double angularVelocity = 0.5,
                      double errorRange      = 0.25);

  // auto movement
  void autoPilotLaser(double forwardVelocity = 0.5, double angularVelocity = 1.0);
};

#endif
