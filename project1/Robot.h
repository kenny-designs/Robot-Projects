#ifndef ROBOT_H
#define ROBOT_H
#pragma once

#include <libplayerc++/playerc++.h>

/**
 * Wrapper class to used to simplify use of the Robot
 */ 
class Robot
{
  PlayerCc::PlayerClient robot;
  PlayerCc::Position2dProxy pp; // The 2D proxy is used to send motion commands.

  // TODO: is there a way we can extract this value instead of hard coding it here?
  // the interval that the simulation is moving at in seconds.
  // By default, the interval is 100 milliseconds as per the
  // How to Use Player/Stage guide at
  // http://playerstage.sourceforge.net/doc/playerstage_instructions_2.0.pdf
  static const double INTERVAL_SIM = 0.1;

  void moveAndRotateOverTicks(double forwardVelocity, double angularVelocity, int ticks);
  int getTicksToCoverDistance(double distance, double velocity);

  public:
  Robot(std::string hostname = "localhost");

  void setMotorEnable(bool isMotorEnabled);
  void moveForwardByMeters(double distanceInMeters, double forwardVelocity = 0.1);
  void rotateByRadians(double radiansToRotate, double angularVelocity = 0.1);
};

#endif
