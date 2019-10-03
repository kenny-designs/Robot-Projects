#ifndef ROBOT_H
#define ROBOT_H
#pragma once

#include <libplayerc++/playerc++.h>

/**
 * Struct used to represent a vector in 2D space
 * TODO: should this be placed into its own file?
 */
struct Vector2 
{
  double x, y;

  Vector2(double xPos, double yPos) : x(xPos), y(yPos) {};

  // TODO: implement when Vector2 has been refactored out of Robot.h
  //double getMagnitude() { return hypot(x, y); };

  /** Overload - operator to subtract the Vector2 by another Vector2 */
  Vector2 operator-(Vector2 const& subtrahend)
  { 
    return Vector2(x - subtrahend.x, y - subtrahend.y);
  }

  /** Overload -= operator to subtract the Vector2 by another Vector2 */
  Vector2& operator-=(Vector2 const& subtrahend)
  { 
    this->x -= subtrahend.x;
    this->y -= subtrahend.y;
    return *this;
  }

  /** Overload / operator to divide the Vector2 by a single divisor */
  Vector2 operator/(double divisor)
  {
    return Vector2(x / divisor, y / divisor);
  }

  /** Overload /= operator to divide the Vector2 by a single divisor */
  Vector2& operator/=(double divisor)
  { 
    this->x /= divisor;
    this->y /= divisor;
    return *this;
  }
};

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
