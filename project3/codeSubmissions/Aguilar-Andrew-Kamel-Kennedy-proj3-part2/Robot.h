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
 * depending 
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
  PlayerCc::BumperProxy     bp;         // The bumper proxy reads from the bumpers.
  PlayerCc::Position2dProxy pp;         // The 2D proxy is used to send motion commands.
  PlayerCc::LaserProxy      sp;         // Laser proxy used to scan the environment
  bool isSimulation;                    // if false, adjusts settings to better accomodate the actual robot
  bool isHandlingBump;                  // true if the robot is currently correcting its position due to a bumper press

  // By default, the interval is 100 milliseconds as per the
  // How to Use Player/Stage guide at
  // http://playerstage.sourceforge.net/doc/playerstage_instructions_2.0.pdf
  static const double INTERVAL_SIM = 0.1;

  // Due to restraints with the robots actuators (i.e. its wheels),
  // we must scale the ticks calculated from the method getFinalTicksAndVelocity()
  // to sync with how the robot is actually moving.
  // These are only applied if isSimulation is set to false
  static const double MOVEMENT_TICK_SCALE = 0.5;
  static const double ROTATION_TICK_SCALE = 0.25;

  // movement
  void moveAndRotateOverTicks(double forwardVelocity, double angularVelocity, int ticks);
  void getFinalTicksAndVelocity(double distance, double& velocity, int& ticks);

  // waypoint movement
  void getAngleDistanceToWaypoint(Vector2& wp, double& angle, double& distance);
  bool hasReachedWaypoint(Vector2& wp, double errorRange);

public:
  // constructor
  Robot(bool isSimulation = true, std::string hostname = "localhost");

  // get position based on odometer
  double getXPos();
  double getYPos();
  double getYaw();

  // check status of bumpers
  bool isLeftPressed();
  bool isRightPressed();
  bool isAnyPressed();

  // print information about the robot
  void printPosition();
  void printBumper();
  void printLaserData();

  // motor
  void setMotorEnable(bool isMotorEnabled);

  // handle basic movement
  void moveForwardByMeters(double distanceInMeters, double forwardVelocity = 0.5);
  void rotateByRadians(double radiansToRotate, double angularVelocity = 0.5);
  void setSpeed(double forwardVelocity = 0.5, double angularVelocity = 0.5, TurnDirection::Enum dir = TurnDirection::Left);

  // bumper movement
  void handleBump(HandleBumpConfig bumpConfig = HandleBumpConfig(),
                  double angle = 5.0 * M_PI / 12.0,  // 75 degrees or 5/12*PI radians
                  double distance = 1.0,
                  double velocity = 0.5,
                  double angularVelocity = 0.5);

  // handle waypoint movement
  void moveToWaypoint(Vector2& wp, double velocity = 0.5, double angularVelocity = 0.5, double errorRange = 0.25);

  // auto movement
  void autoPilotLaser(double forwardVelocity = 0.5, double angularVelocity = 1.0);
};

#endif
