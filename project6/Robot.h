#ifndef ROBOT_H
#define ROBOT_H
#pragma once

// mathematical constants to help with rotating the robot in radians e.g. M_PI
#define _USE_MATH_DEFINES

#include <libplayerc++/playerc++.h>
#include <cmath>
#include "Vector2.h"

// forward declarations
class BumperEventState;

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
 * The method the robot should use to determine where it is
 */
namespace PositionMethod
{
  enum Enum { Localization, Odometry };
}

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
  PositionMethod::Enum posMethod;       // the default way for the robot to determine it's location
  Vector2 *targetWaypoint;              // used to make sure the robot does not overshoot its movement

  // tick interval of the robot
  const double TICK_INTERVAL;

  // scale movement and rotation of the robot to ensure accurate locomotion
  const double MOVEMENT_SCALE,
               ROTATION_SCALE;

  // movement
  bool moveAndRotateOverTicks(double forwardVelocity, double angularVelocity, int ticks);
  void getFinalTicksAndVelocity(double distance, double& velocity, int& ticks);

  // waypoint movement
  double getAngleToWaypoint(Vector2& wp);
  double getDistanceToWaypoint(Vector2& wp);

public:
  // constructor
  Robot(bool   isUsingLaser            = true,
        double movementScale           = 1.0,
        double rotationScale           = 1.0,
        PositionMethod::Enum posMethod = PositionMethod::Localization,
        double tickInterval            = 0.1,
        std::string hostname           = "localhost");

  // destructor
  ~Robot();

  // read from the environment
  void read();

  // utility
  double clampYawToPi(double yaw);

  // get position based on odometer
  Vector2 getOdometerPos();
  double getOdometerYaw();

  // get position based on localization
  Vector2 getLocalizedPos();
  double getLocalizedYaw();

  // obtains the robot's position depending on the posMethod member variable
  Vector2 getPos();
  double getYaw();

  // get status of bumpers
  bool isLeftPressed();
  bool isRightPressed();
  bool isBothPressed();
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
  bool moveForwardByMeters(double distanceInMeters, double forwardVelocity = 0.5);
  bool rotateByRadians(double radiansToRotate, double angularVelocity = 0.5);
  void dislodgeFromObstacle(double distance, double velocity);

  // wrapper method for pp.SetSpeed()
  void setSpeed(double forwardVelocity  = 0.5,
                double angularVelocity  = 0.5,
                TurnDirection::Enum dir = TurnDirection::Left);

  // handle waypoint movement
  bool hasReachedWaypoint(Vector2& wp, double errorRange = 0.1);
  void rotateToFaceWaypoint(Vector2& wp, double angularVelocity = 0.5, double errorRange = 0.0175);
  void moveToWaypoint(Vector2& wp,
                      BumperEventState& bumperEventState,
                      double velocity        = 0.5,
                      double angularVelocity = 0.5,
                      double errorRange      = 0.25);

  // auto-pilot movement
  void autoPilotLaser(int tickDuration = INT_MAX, double forwardVelocity = 0.5, double angularVelocity = 1.0);
};

/**
 * Interface for handling bumper events.
 */
struct BumperEventState
{
  double distance, velocity, angularVelocity;

  BumperEventState(double distance, double velocity, double angularVelocity);

  virtual void handleBump(Robot *robot) = 0;
};

/**
 * Corrects the robot's position by having it backup, rotate, then move forward
 * depending on which bumper was pressed.
 */
struct SimpleBumper : public BumperEventState
{
  double angle;              // angle to rotate when correcting position
  TurnDirection::Enum both,  // direction to turn if both bumpers hit
                      left,  // direction to turn if left bumper hit
                      right; // direction to turn if right bumper hit

  SimpleBumper(TurnDirection::Enum both  = TurnDirection::Random,
               TurnDirection::Enum left  = TurnDirection::Right,
               TurnDirection::Enum right = TurnDirection::Left,
               double angle              = 5.0 * M_PI / 12.0, // ~75 degrees
               double distance           = 0.75,
               double velocity           = 1.0,
               double angularVelocity    = 1.0);

  void handleBump(Robot *robot);
};

/**
 * Corrects the robot's position by having it backup followed by
 * engaging laser guided auto-pilot movement for a set number of ticks.
 */
struct AutoPilot : public BumperEventState
{
  int ticks; // the number of ticks to apply auto pilot

  AutoPilot(int    ticks           = 50,
            double distance        = 0.5,
            double velocity        = 0.5,
            double angularVelocity = 1.0);

  void handleBump(Robot *robot);
};

#endif
