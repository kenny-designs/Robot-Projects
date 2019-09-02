#include "Robot.h"
#include <cstdlib>
#include <iostream>
#include <string>

/**
 * Set up proxy. Proxies are the datastructures that Player uses to
 * talk to the simulator and the real robot.
 *
 * @param hostname - address to connect to (localhost by default)
 */
// TODO: Might be better to make forwardSpeed and turnRate parameters
Robot::Robot(std::string hostname) : robot(hostname), pp(&robot, 0), 
                                     forwardSpeed(0.1), turnRate(0.1) {}

/**
 * Move and rotate the robot over the given number of ticks
 *
 * @param forwardVelocity - forward velocity in meters per second
 * @param angularVelocity - angular velocity in radians per second
 * @param ticks - number of ticks to move/rotate the robot for
 */
void Robot::moveAndRotateOverTicks(double forwardVelocity, double angularVelocity, int ticks)
{
  // Enter movement control loop
  for (int curTick = 0; curTick < ticks; ++curTick)
  {
    // Read from the proxies.
    robot.Read();

    std::cout << "Speed: " << forwardVelocity << "\n";      
    std::cout << "Turn rate: " << angularVelocity  << "\n";

    // Send the motion commands that we decided on to the robot.
    pp.SetSpeed(forwardVelocity, angularVelocity);
  }
}

/**
 * Enable or disable the robot's motor
 *
 * @param isMotorEnabled - true to enable the motor, false to disable
 */ 
void Robot::setMotorEnable(bool isMotorEnabled)
{
  // Allow the program to take charge of the motors (take care now)
  pp.SetMotorEnable(isMotorEnabled);
}

/**
 * Move the robot by the given distance in meters
 *
 * @param distanceInMeters - total distance to move in meters
 */
void Robot::moveForwardByMeters(double distanceInMeters)
{
  // TODO: Can this be simplified?
  double forwardVelocity = forwardSpeed;
  if (distanceInMeters < 0)
  {
    forwardVelocity *= -1;
    distanceInMeters *= -1;
  }

  int ticks = (int)(distanceInMeters / forwardSpeed / 0.1); // TODO: replace with const for sim interval
  moveAndRotateOverTicks(forwardVelocity, 0, ticks);
}

/**
 * Rotates the robot counter-clockwise in radians
 *
 * @param radiansToRotate - total angular distance to rotate in radians
 */ 
void Robot::rotateByRadians(double radiansToRotate)
{
  // TODO: Can this be simplified?
  double angularVelocity = turnRate;
  if (radiansToRotate < 0)
  {
    angularVelocity *= -1;
    radiansToRotate *= -1;
  }

  int ticks = (int)(radiansToRotate / turnRate / 0.1); // TODO: replace with const for sim interval
  moveAndRotateOverTicks(0, angularVelocity, ticks);
}
