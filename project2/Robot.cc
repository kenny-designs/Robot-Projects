// mathematical constants to help with rotating the robot in radians e.g. M_PI
#define _USE_MATH_DEFINES

#include "Robot.h"
#include "Vector2.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <limits>   // std::numeric_limits
#include <cmath>    // acos(), cos(), sin()
#include <stdlib.h> // srand, rand
#include <time.h>   // time

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
  isSimulation(isSimulation),
  isCorrectingPosition(false),
  robot(hostname),
  pp(&robot, 0),
  bp(&robot, 0) {}

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
    // read from proxies
    robot.Read();

    // break if bumper hit and we're not currently correcting the position
    if (!isCorrectingPosition && (isLeftPressed() || isRightPressed())) break;
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
 * Generates the angle and distance needed to first rotate to face then
 * approach the given waypoint.
 *
 * @param wp       - the waypoint we want to move to as a Vector2
 * @param angle    - angle the robot must turn to face the waypoint
 * @param distance - distance the robot must travel to reach the waypoint
 */ 
void Robot::getAngleDistanceToWaypoint(Vector2& wp, double& angle, double& distance)
{
  // obtain both the direction and position vector of the robot
  Vector2 dir(cos(getYaw()), sin(getYaw())),
          pos(getXPos(), getYPos());

  // center waypoint to the origin then normalize it
  Vector2 wpNorm = wp - pos;
  Vector2::normalize(wpNorm);

  // calculate the distance between the robot and the given waypoint
  distance = Vector2::getMagnitude(pos - wp);

  // find the angle to rotate the robot so that it faces the given waypoint
  angle = acos(wpNorm.x * dir.x + wpNorm.y * dir.y);

  // if angle is nan, return
  // TODO: this isn't very clean. Find a way to protect against nan
  if (isnan(angle)) return;

  // zed value for cross product. If negative, flip angle
  if (wpNorm.x * dir.y - wpNorm.y * dir.x > 0) { angle *= -1; }
}

/**
 * Corrects the robots position if a bumper has been pressed
 */
void Robot::handleBump()
{
  isCorrectingPosition = true;

  robot.Read();
  bool isLeft  = isLeftPressed(),
       isRight = isRightPressed();

  printBumper();

  double angle = M_PI_4;  // default rotate left
  if (isLeft && isRight)
  {
    // initialize random seed
    srand(time(NULL));

    // rotate in random direction based on rand()
    angle *= rand() % 2 ? 1 : -1;

    // report direction we are rotating
    // TODO: remove
    std::cout << "Now turning: " << (angle < 0 ? "right" : "left") << "\n";
  }
  else if (isLeft)
  {
    angle *= -1;
  }

  // correct robot position
  moveForwardByMeters(-0.5, 0.5);  // back up by 0.5 meters
  rotateByRadians(angle, 0.5);     // rotate by the angle
  moveForwardByMeters(0.5, 0.5);   // move forward by 0.5 meters

  isCorrectingPosition = false;

  // TODO: remove
  std::cout << "\n";
}

/**
 * Returns true if the robot has reached the given waypoint within the
 * given error range
 *
 * @param wp         - the waypoint we are testing to see if we reached
 * @param errorRange - the range in which the robot must be to the wp
 * @return           - true if the robot has reached the robot. Otherwise, false
 */ 
bool Robot::hasReachedWaypoint(Vector2& wp, double errorRange)
{
  // obtain the robots x and y position
  Vector2 pos(getXPos(), getYPos());

  // return true if reached the waypoint within the error range
  return (pos.x + errorRange > wp.x && pos.x - errorRange < wp.x) &&
         (pos.y + errorRange > wp.y && pos.y - errorRange < wp.y);
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
bool Robot::isLeftPressed()
{
  return bp[0];
}

/**
 * Returns true if the right bumper is pressed
 * @return True if right bumper pressed
 */
bool Robot::isRightPressed()
{
  return bp[1];
}

/** Prints the X, Y, and Yaw positions of the Robot */
void Robot::printPosition()
{
  robot.Read();
  std::cout << "Robot x position: " << getXPos() << "\n" <<
               "      y position: " << getYPos() << "\n" <<
               "      yaw:        " << getYaw()  << "\n\n";
}

/** Prints state of the bumpers */
void Robot::printBumper()
{
  // must read data from the server to find bumper state
  robot.Read();
  std::cout << "Left  bumper pressed:  " << isLeftPressed()  << "\n" <<
               "Right bumper pressed:  " << isRightPressed() << "\n";
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
  // move to waypoint wp until within 0.5m of it
  bool isAtDestination = hasReachedWaypoint(wp, 0.25);
  while (!isAtDestination)
  {
    double angle, distance;
    getAngleDistanceToWaypoint(wp, angle, distance);

    if (isnan(angle)) return;

    // rotate towards then travel to the given waypoint
    rotateByRadians(angle, 0.5);
    moveForwardByMeters(distance, 0.5);

    // check again if the robot has reached the destination
    isAtDestination = hasReachedWaypoint(wp, 0.25);

    // if not yet at the destination, correct position then try again
    if (!isAtDestination)
    {
      handleBump();
    }
  }
}
