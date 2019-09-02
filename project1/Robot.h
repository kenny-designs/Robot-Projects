/**
 * Wrapper class to used to simplify use of the Robot
 */ 

#ifndef ROBOT_H
#define ROBOT_H
#pragma once

#include <libplayerc++/playerc++.h>

class Robot
{
  // TODO: Can we give it a better name? Or perhaps rename the class altogether
  PlayerCc::PlayerClient robot;
  PlayerCc::Position2dProxy pp; // The 2D proxy is used to send motion commands.

  double forwardSpeed;  // forward speed in meters per second
  double turnRate;      // turn rate in radians per second

  void moveAndRotateOverTicks(double forwardVelocity, double angularVelocity, int ticks);

  public:
  Robot(std::string hostname = "localhost");

  void setMotorEnable(bool isMotorEnabled);

  void moveForwardByMeters(double distanceInMeters);
  void rotateByRadians(double radiansToRotate);
};

#endif
