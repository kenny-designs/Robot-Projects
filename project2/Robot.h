#ifndef ROBOT_H
#define ROBOT_H
#pragma once

#include <libplayerc++/playerc++.h>
#include "Vector2.h" // TODO: should we use a pointer instead of including?

/**
 * Wrapper class used to simplify use of the Robot
 */ 
class Robot
{
  PlayerCc::PlayerClient    robot;
  PlayerCc::BumperProxy     bp;         // The bumper proxy reads from the bumpers.
  PlayerCc::Position2dProxy pp;         // The 2D proxy is used to send motion commands.
  bool isSimulation;                    // if false, adjusts settings to better accomodate the actual robot

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

  void moveAndRotateOverTicks(double forwardVelocity, double angularVelocity, int ticks);
  void getFinalTicksAndVelocity(double distance, double& velocity, int& ticks);
  void getAngleDistanceToWaypoint(Vector2& wp, double& angle, double& distance);

public:
  // constructor
  Robot(bool isSimulation = true, std::string hostname = "localhost");

  // get position based on odometer
  double getXPos();
  double getYPos();
  double getYaw();

  // check if left or right bumper is pressed
  bool isLeftBumper();
  bool isRightBumper();

  // print information about the robot
  void printPosition();
  void printBumper();

  // motor
  void setMotorEnable(bool isMotorEnabled);

  // handle basic movement
  void moveForwardByMeters(double distanceInMeters, double forwardVelocity = 0.1);
  void rotateByRadians(double radiansToRotate, double angularVelocity = 0.1);

  // handle waypoint movement
  void moveToWaypoint(Vector2& wp);
};

#endif