// mathematical constants to help with rotating the robot in radians e.g. M_PI
#define _USE_MATH_DEFINES

#include "Robot.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <limits> // std::numeric_limits
#include <cmath>  // hypot(), acos(), cos(), sin()

// used for comparing doubles to 0
#define EPSILON std::numeric_limits<double>::epsilon()

/**
 * Set up proxy. Proxies are the datastructures that Player uses to
 * talk to the simulator and the real robot.
 *
 * @param isSimulation - if false, adjusts settings to better accomodate the actual robot (true by default)
 * @param hostname     - address to connect to (localhost by default)
 */
Robot::Robot(bool isSimulation, std::string hostname) :
  isSimulation(isSimulation), robot(hostname), pp(&robot, 0), bp(&robot, 0) {}

/**
 * Move and rotate the robot over the given number of ticks
 *
 * @param forwardVelocity - forward velocity to move in meters per second
 * @param angularVelocity - angular velocity to rotate in radians per second
 * @param ticks           - number of ticks to apply the forward/angular velocities to the robot for
 */
void Robot::moveAndRotateOverTicks(double forwardVelocity, double angularVelocity, int ticks)
{
  // Send the motion commands that we decided on to the robot.
  pp.SetSpeed(forwardVelocity, angularVelocity);

  // Enter movement control loop
  for (int curTick = 0; curTick < ticks; ++curTick)
  {
    // Read from the proxies.
    robot.Read();

    // TODO: uncomment when finished debugging. Or remove altogether
    // Report current forward and angular velocities
    // std::cout << "Forward Velocity: " << forwardVelocity << "\tm/s\n";      
    // std::cout << "Angular Velocity: " << angularVelocity  << "\trad/s\n\n";
  }

  // stop moving
  pp.SetSpeed(0, 0);
}

/**
 * Calculates what the velocity and ticks should be to cover the given distance. The given velocity
 * and ticks are mutated to match this calculation.
 *
 * The velocity variable will remain the same unless a negative distance is given. Be that the
 * case, velocity will be negated. Its absolute value will never change though.
 *
 * The ticks variable will be approximated to match the number of ticks needed to cover the given
 * distance with our current velocity.
 *
 * For example, to cover 1 meter while traveling at 0.1 m/s, we divide
 * 1m by 0.1m/s to get 10s. We then divide that result by the number of ticks
 * that occur in a second (default is 10 as per INTERVAL_SIM). So 10s divided
 * by 0.1s is 100. So, for the robot to travel 1 meter while going at 0.1 m/s,
 * it will take a total of 100 ticks.
 *
 * @param  distance - total distance to travel
 * @param  velocity - rate that of which we are traveling. Original variable will be negated
 *                    if traveling a negative distance
 * @param ticks     - mutates the original variable with the number of ticks needed to cover
 *                    the given distance at the given velocity
 */ 
void Robot::getFinalTicksAndVelocity(double distance, double& velocity, int& ticks)
{
  // if distance or velocity is 0, return for there is nothing to be done
  if (fabs(distance) < EPSILON || fabs(velocity) < EPSILON) { return; }

  // if negative distance, negate the velocity
  if (distance < 0) { velocity *= -1; }

  ticks = abs((int)(distance / velocity / INTERVAL_SIM));
}

/**
 * Gets Robot X position based on Position2dProxy
 * @return X position as double
 */
double Robot::getXPos()
{
  robot.Read();
  return pp.GetXPos();
}

/**
 * Gets Robot Y position based on Position2dProxy
 * @return Y position as double
 */
double Robot::getYPos()
{
  robot.Read();
  return pp.GetYPos();
}

/**
 * Gets Robot Yaw rotation based on Position2dProxy
 * @return Yaw rotation as double
 */
double Robot::getYaw()
{
  robot.Read();
  return pp.GetYaw();
}

/**
 * Returns true if the left bumper is pressed
 * @return True if left bumper pressed
 */
// TODO: rename to isLeftPressed
bool Robot::isLeftBumper()
{
  robot.Read();
  return bp[0];
}

/**
 * Returns true if the right bumper is pressed
 * @return True if right bumper pressed
 */
// TODO: rename to isRightPressed
bool Robot::isRightBumper()
{
  robot.Read();
  return bp[1];
}

/** Prints the X, Y, and Yaw positions of the Robot */
void Robot::printPosition()
{
  robot.Read();
  std::cout << "x: " << getXPos() << "\n" <<
               "y: " << getYPos() << "\n" <<
               "a: " << getYaw()  << "\n";
}

/** Prints state of the bumpers */
void Robot::printBumper()
{
  // must read data from the server to find bumper state
  robot.Read();
  std::cout << "Left  bumper: " << isLeftBumper()  << "\n" <<
               "Right bumper: " << isRightBumper() << "\n";
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
  int ticks;
  getFinalTicksAndVelocity(distanceInMeters, forwardVelocity, ticks);

  // adjust for actual robot if needed
  if (!isSimulation) { ticks *= MOVEMENT_TICK_SCALE; }

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
  int ticks;
  getFinalTicksAndVelocity(radiansToRotate, angularVelocity, ticks);

  // adjust for actual robot if needed
  if (!isSimulation) { ticks *= ROTATION_TICK_SCALE; }

  moveAndRotateOverTicks(0, angularVelocity, ticks);
}

/**
 * The robot will move to the specified Waypoint even if obstacles are in the way
 *
 * @param wp - the waypoint for the robot to move to
 */ 
void Robot::moveToWaypoint(Vector2& wp)
{
  double yaw  = getYaw();               // yaw of the robot in radians
  Vector2 dir(cos(yaw), sin(yaw));      // create direction unit vector based on the robot's yaw
  Vector2 pos(getXPos(), getYPos());    // create a vector based on the robot's x and y position

  // find the angle to rotate the robot so that it faces the given waypoint
  Vector2 wpNorm = wp - pos;                                    // center the given waypoint to the origin
  wpNorm                /= hypot(wpNorm.x, wpNorm.y);           // normalize by dividing by its magnitude
  double dotProduct      = wpNorm.x * dir.x + wpNorm.y * dir.y; // dot product of normalized waypoint and direction vector
  double angle           = acos(dotProduct);                    // calculate angle between waypoint and direction vector

  // if angle is nan, return
  if (isnan(angle)) return;

  // ensure the robot rotates in the right direction
  if ((dir.x > 0 != wpNorm.y > 0) &&
      !(dir.x < 0 && dir.y < 0 && wpNorm.x > 0 && wpNorm.y > 0))
  {
    angle *= -1;
  }

  std::cout << "dir.x:    " << (dir.x > 0)    << "\n" <<
               "dir.y:    " << (dir.y > 0)    << "\n" <<
               "wpNorm.x: " << (wpNorm.x > 0) << "\n" <<
               "wpNorm.y: " << (wpNorm.y > 0) << "\n" <<
               "Turning:  " << (angle > 0 ? "Left" : "right") << "\n\n";

  // to calculate the distance the robot must travel, subtract the waypoint
  // from the robot's position then find the magnitude
  // TODO: this can be cleaned by adding a magnitude method to the Vector2 struct
  double distance = hypot(pos.x - wp.x, pos.y - wp.y);

  // print the angle and distance that the robot will traverse
  //std::cout << "Angle:    " << angle << "\n";
  //std::cout << "Distance: " << distance << "\n";

  rotateByRadians(angle, 0.5);               // rotate towards the given waypoint
  moveForwardByMeters(distance, 0.5);        // travel to the given waypoint

  // print final position
  //printPosition();
}
