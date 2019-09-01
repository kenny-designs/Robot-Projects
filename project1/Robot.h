/**
 * Wrapper class to used to simplify use of the Robot
 */ 

#ifndef ROBOT_H
#define ROBOT_H
#pragma once

#include <libplayerc++/playerc++.h>

class Robot
{
  PlayerCc::PlayerClient robot;
  PlayerCc::Position2dProxy pp; // The 2D proxy is used to send motion commands.

  public:
  Robot(std::string hostname = "localhost");

  void setMotorEnable(bool isMotorEnabled);
  void moveForwardOverTicks(double forwardSpeed, int ticks);
  void rotateOverTicks(double turnRate, int ticks);
};

#endif
