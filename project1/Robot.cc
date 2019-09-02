#include "Robot.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <limits> // std::numeric_limits

// used for comparing doubles to 0
#define EPSILON std::numeric_limits<double>::epsilon()

/**
 * Set up proxy. Proxies are the datastructures that Player uses to
 * talk to the simulator and the real robot.
 *
 * @param hostname - address to connect to (localhost by default)
 */
Robot::Robot(std::string hostname) : robot(hostname), pp(&robot, 0) {}

/**
 * Move and rotate the robot over the given number of ticks
 *
 * @param forwardVelocity - forward velocity to move in meters per second
 * @param angularVelocity - angular velocity to rotate in radians per second
 * @param ticks           - number of ticks to apply the forward/angular velocities to the robot for
 */
void Robot::moveAndRotateOverTicks(double forwardVelocity, double angularVelocity, int ticks)
{
  // Enter movement control loop
  for (int curTick = 0; curTick < ticks; ++curTick)
  {
    // Read from the proxies.
    robot.Read();

    // Report current forward and angular velocities
    std::cout << "Forward Velocity: " << forwardVelocity << "\tm/s\n";      
    std::cout << "Angular Velocity: " << angularVelocity  << "\trad/s\n\n";

    // Send the motion commands that we decided on to the robot.
    pp.SetSpeed(forwardVelocity, angularVelocity);
  }
}

/**
 * Approximates the total number of ticks it will take to cover a given
 * distance while applying a given velocity.
 *
 * For example, to cover 1 meter while traveling at 0.1 m/s, we divide
 * 1m by 0.1m/s to get 10s. We then divide that result by the number of ticks
 * that occur in a second (default is 10 as per INTERVAL_SIM). So 10s divided
 * by 0.1s is 100. So, for the robot to travel 1 meter while going at 0.1 m/s,
 * it will take a total of 100 ticks.
 *
 * @param  distance - total distance to travel
 * @param  velocity - rate that of which we are traveling
 * @return            approximate number of ticks to travel the distance with velocity
 */ 
int Robot::getTicksToCoverDistance(double distance, double velocity)
{
  return abs((int)(distance / velocity / INTERVAL_SIM));
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
 * @param distanceInMeters - total distance to move in meters. Negative values move backwards
 * @param forwardVelocity  - rate to move forward in meters per second (default 0.1). Negative values
 *                           move backwards
 */
void Robot::moveForwardByMeters(double distanceInMeters, double forwardVelocity)
{
  // if distance or velocity is 0, return for there is no moving that can be done
  if (fabs(distanceInMeters) < EPSILON || fabs(forwardVelocity) < EPSILON) { return; }

  // if negative, negate forward velocity
  if (distanceInMeters < 0) { forwardVelocity *= -1; }

  int ticks = getTicksToCoverDistance(distanceInMeters, forwardVelocity);
  moveAndRotateOverTicks(forwardVelocity, 0, ticks);
}

/**
 * Rotates the robot counter-clockwise in radians
 *
 * @param radiansToRotate - total angular distance to rotate in radians. Negative values rotate clockwise
 * @param angularVelocity - rate to rotate in radians per second (default 0.1). Negative values
 *                          rotate clockwise
 */ 
void Robot::rotateByRadians(double radiansToRotate, double angularVelocity)
{
  // if radiansToRotate or velocity is 0, return for there is no rotating that can be done
  if (fabs(radiansToRotate) < EPSILON || fabs(angularVelocity) < EPSILON) { return; }

  // if negative rotation, negate angular velocity
  if (radiansToRotate < 0) { angularVelocity *= -1; }

  int ticks = getTicksToCoverDistance(radiansToRotate, angularVelocity);
  moveAndRotateOverTicks(0, angularVelocity, ticks);
}
