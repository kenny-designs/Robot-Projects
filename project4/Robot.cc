#include "Robot.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <limits>   // std::numeric_limits
#include <stdlib.h> // srand, rand
#include <time.h>   // time
#include <cstdio>   // printf

// used for comparing doubles to 0
#define EPSILON std::numeric_limits<double>::epsilon()

/**
 * Set up proxy. Proxies are the datastructures that Player uses to
 * talk to the simulator and the real robot.
 *
 * @param isUsingLaser  - if true, sets up the LaserProxy to be used by the robot
 * @param movementScale - scales the overall velocity of the robot
 * @param rotationScale - scales the overall angular velocity of the robot
 * @param tickInterval  - the interval that of which the robot ticks at
 * @param hostname      - address to connect to
 */
Robot::Robot(bool isUsingLaser, double movementScale, double rotationScale, double tickInterval,  std::string hostname) :
  isHandlingBump(false),
  MOVEMENT_SCALE(movementScale),
  ROTATION_SCALE(rotationScale),
  TICK_INTERVAL(tickInterval),
  robot(hostname),
  pp(&robot, 0),
  bp(&robot, 0),
  lp(&robot, 0)
{
  // initial read to prevent segmentation defaults with proxies
  robot.Read();

  // turn on the motor
  setMotorEnable(true);

  // laser setup
  sp = isUsingLaser ? new PlayerCc::LaserProxy(&robot, 0) : NULL;
}

/** Destructor used to release memory */
Robot::~Robot()
{
  // turn off the motor
  setMotorEnable(false);

  // free memory used by the laser proxy
  delete sp;

  // Have Mr. Robot say goodbye for it is the polite thing to do
  std::cout << "\nPowering off. Goodbye! o7\n";
}

/** Read from the environment */
void Robot::read()
{
  robot.Read();
}

/**
 * Move and rotate the robot over the given number of ticks
 *
 * @param forwardVelocity - forward velocity to move in meters per second
 * @param angularVelocity - angular velocity to rotate in radians per second
 * @param ticks           - number of ticks to apply the forward/angular velocities to the robot for
 */
void Robot::moveAndRotateOverTicks(double forwardVelocity, double angularVelocity, int ticks)
{
  // scale velocity for proportional control
  double velocityScale;

  // double ticks to account for proportional control
  ticks *= 2;

  // Enter movement control loop
  for (int curTick = 1; curTick <= ticks; ++curTick)
  {
    // read from proxies
    robot.Read();

    // scale velocity based on current and max ticks
    velocityScale = (1.0 - ((double)curTick) / ((double)ticks));

    // move the robot
    pp.SetSpeed(forwardVelocity * velocityScale, angularVelocity * velocityScale);
    
    // break if a bumper has been pressed and we're not currently handling a bumper event
    if (!isHandlingBump && isAnyPressed()) break;
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
 * that occur in a second (default is 10 as per TICK_INTERVAL). So 10s divided
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
  if (fabs(distance) < EPSILON || fabs(velocity) < EPSILON)
  {
    ticks = 0;
    return;
  }

  // if negative distance, negate the velocity
  if (distance < 0)
  {
    velocity *= -1;
  }

  ticks = abs((int)(distance / velocity / TICK_INTERVAL)); 
}

/**
 * Generates the angle and distance needed to reach a given waypoint.
 *
 * @param pos      - the current position of the robot
 * @param yaw      - the current yaw of the robot
 * @param wp       - the waypoint we want to move to as a Vector2
 * @param angle    - angle the robot must turn to face the waypoint
 * @param distance - distance the robot must travel to reach the waypoint
 */ 
void Robot::getAngleDistanceToWaypoint(Vector2& pos, double yaw, Vector2& wp, double& angle, double& distance)
{
  // obtain the direction and vector of the robot
  Vector2 dir(cos(yaw), sin(yaw));

  // center waypoint to the origin then normalize it
  Vector2 wpNorm = wp - pos;
  Vector2::normalize(wpNorm);

  // calculate the distance between the robot and the given waypoint
  distance = Vector2::getMagnitude(pos - wp);

  // find the angle to rotate the robot so that it faces the given waypoint
  angle = acos(wpNorm.x * dir.x + wpNorm.y * dir.y);

  // zed value for cross product. If negative, flip angle
  if (wpNorm.x * dir.y - wpNorm.y * dir.x > 0)
  {
    angle *= -1;
  }
}

/**
 * Gets Robot Yaw rotation based on odometry
 * @return Yaw rotation as double
 */
double Robot::getOdometerYaw()
{
  return pp.GetYaw();
}

/**
 * Gets the current position of the robot based off of the odometer
 * @return Vector2 of the robot's current odometer position
 */ 
Vector2 Robot::getOdometerPos()
{
  return Vector2(pp.GetXPos(), pp.GetYPos());
}

/**
 * Gets the current position of the robot based off of localization
 * @return Vector2 of the robot's current localized position
 */ 
Vector2 Robot::getLocalizedPos()
{
  player_pose2d_t pose = getPoseFromLocalizeProxy();

  return Vector2(pose.px, pose.py);
}

/**
 * Gets Robot Yaw rotation based on localization
 * @return Localized Yaw rotation as double
 */ 
double Robot::getLocalizedYaw()
{
  return getPoseFromLocalizeProxy().pa;
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

/**
 * Returns true if any bumper is pressed
 * @return True if any bumper pressed
 */
bool Robot::isAnyPressed()
{
  return bp.IsAnyBumped();
}

/** Prints the X, Y, and Yaw odometry positions of the Robot */
void Robot::printOdometerPosition()
{
  Vector2 pos = getOdometerPos();
  std::cout << "Robot Odometer Position"   << "\n" <<
               "-----------------------"   << "\n" <<
               "X:   " << pos.x            << "\n" <<
               "Y:   " << pos.y            << "\n" <<
               "Yaw: " << getOdometerYaw() << "\n";
}

/** Prints the X, Y, and Yaw positions of the robot based on localization */
void Robot::printLocalizedPosition()
{
  player_pose2d_t pose = getPoseFromLocalizeProxy();
  std::cout << "Robot Localized Position"  << "\n" <<
               "------------------------"  << "\n" <<
               "X:   " << pose.px          << "\n" <<
               "Y:   " << pose.py          << "\n" <<
               "Yaw: " << pose.pa          << "\n";
}

/** Prints all hypotheses */
void Robot::printAllHypotheses()
{
  player_localize_hypoth_t hypothesis;
  player_pose2d_t          pose;
  double                   weight;
  uint32_t                 hCount = lp.GetHypothCount();

  if (hCount < 1) return;

  std::cout << "AMCL gives us " << hCount + 1 << " possible locations:" << "\n";

  for (int i = 0; i <= hCount; i++)
  {
    hypothesis = lp.GetHypoth(i);
    pose       = hypothesis.mean;
    weight     = hypothesis.alpha;
    std::cout << "X: " << pose.px << "\t";
    std::cout << "Y: " << pose.py << "\t";
    std::cout << "A: " << pose.pa << "\t";
    std::cout << "W: " << weight  << "\n";
  }

  std::cout << "\n";
}

/** Prints state of the bumpers */
void Robot::printBumper()
{
  // must read data from the server to find bumper state
  std::cout << "Left  bumper pressed:  " << isLeftPressed()  << "\n" <<
               "Right bumper pressed:  " << isRightPressed() << "\n";
}

/** Prints data from the laser */
void Robot::printLaserData()
{
  if (!sp) return;

  std::cout << "Max laser distance:        " << sp->GetMaxRange() << "\n" <<
               "Number of readings:        " << sp->GetCount()    << "\n" <<
               "Closest thing on left:     " << sp->MinLeft()     << "\n" <<
               "Closest thing on right:    " << sp->MinRight()    << "\n" <<
               "Range of a single point:   " << sp->GetRange(5)   << "\n" <<
               "Bearing of a single point: " << sp->GetBearing(5) << "\n";
}

/**
 * Finds the hypothesis with the greatest weight
 *
 * @return hypothesis with greatest weight
 */ 
player_localize_hypoth_t Robot::getBestLocalizeHypothesis()
{
  player_localize_hypoth_t hypothesis;
  int                      maxIndex;
  double                   weight, maxWeight = 0;

  // Find pose with the most weight
  for (int i = 0; i <= lp.GetHypothCount(); i++)
  {
    hypothesis = lp.GetHypoth(i);
    weight     = hypothesis.alpha;

    if (weight > maxWeight)
    {
      maxWeight = weight;
      maxIndex  = i;
    }
  }

  // Returns hypothesis with the most weight
  return lp.GetHypoth(maxIndex);
}

/**
 * Read the position of the robot from the localization proxy. 
 *
 * The localization proxy gives us a set of "hypotheses", each of
 * which is a number of possible locations for the robot, and from
 * each we extract the mean, which is a pose.
 *
 * As the number of hypotheses drops, the robot should be more sure
 * of where it is.
 *
 * @return the pose with the greatest amount of weight
 */
player_pose2d_t Robot::getPoseFromLocalizeProxy()
{
  return getBestLocalizeHypothesis().mean;
}

/**
 * Robot localizes itself by moving around the map until it is certain of
 * its position
 */ 
void Robot::localize()
{
  // endlessly loop until the robot is certain of where it is
  while (1)
  {
    robot.Read();

    printAllHypotheses();

    // if only one hypothesis remains, return
    if (lp.GetHypothCount() == 1) return;

    // travel backwards in a circle shape
    setSpeed(-0.75, 1.0);
  }
}

/**
 * Enable or disable the robot's motor
 *
 * @param isMotorEnabled - true to enable the motor, false to disable
 */ 
void Robot::setMotorEnable(bool isMotorEnabled)
{
  pp.SetMotorEnable(isMotorEnabled);
}

/**
 * Move the robot by the given distance in meters
 *
 * @param distanceInMeters - total distance to move in meters. Negative values move backwards
 * @param forwardVelocity  - rate to move forward in meters per second. Negative values
 *                           move backwards
 */
void Robot::moveForwardByMeters(double distanceInMeters, double forwardVelocity)
{
  // scale movement
  distanceInMeters *= MOVEMENT_SCALE;

  int ticks;
  getFinalTicksAndVelocity(distanceInMeters, forwardVelocity, ticks);

  moveAndRotateOverTicks(forwardVelocity, 0, ticks);
}

/**
 * Rotates the robot counter-clockwise in radians
 *
 * @param radiansToRotate - total angular distance to rotate in radians. Negative values rotate clockwise
 * @param angularVelocity - rate to rotate in radians per second. Negative values
 *                          rotate clockwise
 */ 
void Robot::rotateByRadians(double radiansToRotate, double angularVelocity)
{
  // scale rotation
  radiansToRotate *= ROTATION_SCALE;

  int ticks;
  getFinalTicksAndVelocity(radiansToRotate, angularVelocity, ticks);

  moveAndRotateOverTicks(0, angularVelocity, ticks);
}

/**
 * Extends the Proximity2dProxy's SetSpeed() method to allow for more flexibility when
 * rotating the robot
 *
 * @param forwardVelocity - forward velocity to move the robot in m/s
 * @param angularVelocity - angular velocity to rotate the robot in rad/s
 * @param dir             - direction for the robot to move in
 */ 
void Robot::setSpeed(double forwardVelocity, double angularVelocity, TurnDirection::Enum dir)
{
  switch(dir)
  {
    case TurnDirection::Left:
      angularVelocity = angularVelocity < 0 ? -angularVelocity : angularVelocity;
      break;

    case TurnDirection::Right:
      angularVelocity = angularVelocity > 0 ? -angularVelocity : angularVelocity;
      break;

    case TurnDirection::None:
      angularVelocity = 0;
      break;
  }

  pp.SetSpeed(forwardVelocity, angularVelocity);
}

/**
 * Corrects the robot's position if a bumper has been pressed by adjusting its
 * orientation and location based on the given parameters.
 *
 * @param bumpConfig  - how the robot should respond to bumpers being pressed
 * @param angle       - the angle to rotate the robot
 * @param distance    - the distance for the robot to move
 * @param velocity    - the velocity to move at
 * @param angVelocity - the velocity to rotate at
 */
void Robot::handleBump(HandleBumpConfig bumpConfig,
                       double angle, double distance,
                       double velocity, double angVelocity)
{
  robot.Read();
  bool isLeft  = isLeftPressed(),
       isRight = isRightPressed();

  // return if neither bumper is pressed
  if (!(isLeft || isRight)) return;

  TurnDirection::Enum dir;

  // determine how to rotate the robot
  if      (isLeft && isRight) dir = bumpConfig.both;
  else if (isLeft)            dir = bumpConfig.left;
  else                        dir = bumpConfig.right;

  // adjust the angle depending on the robot should rotate
  switch(dir)
  {
    case TurnDirection::Random:
      srand(time(NULL));
      angle *= rand() % 2 ? 1 : -1;
      break;

    case TurnDirection::Right:
      angle *= -1;
      break;

    case TurnDirection::None:
      angle = 0;
      break;
  }

  // robot is now addressing bumper press event
  isHandlingBump = true;

  // correct robot position by dislodging it
  moveForwardByMeters(-distance, velocity);  // back up by the given distance
  rotateByRadians(angle, angVelocity);       // rotate by the given angle
  moveForwardByMeters(distance, velocity);   // move forward by the given distance

  // robot has finished addressing the bumper press event
  isHandlingBump = false;
}

/**
 * Returns true if the robot has reached the given waypoint within the
 * given error range
 *
 * @param pos        - the waypoint the robot is currently at
 * @param wp         - the waypoint we are testing to see if we reached
 * @param errorRange - the range in which the robot must be to the wp
 * @return true if the robot has reached the robot. Otherwise, false
 */ 
bool Robot::hasReachedWaypoint(Vector2& pos, Vector2& wp, double errorRange)
{
  // return true if reached the waypoint within the error range
  return (pos.x + errorRange > wp.x && pos.x - errorRange < wp.x) &&
         (pos.y + errorRange > wp.y && pos.y - errorRange < wp.y);
}

/**
 * The robot will move to the specified Waypoint even if obstacles are in the way
 *
 * @param wp              - the waypoint for the robot to move to
 * @param useLocalization - uses localization if set to true. Otherwise, use odometry
 * @param velocity        - velocity for the robot to move in m/s
 * @param angularVelocity - angular velocity for the robot to rotate in rad/s
 * @param errorRange      - minimum distance robot must be from waypoint in meters
 */ 
void Robot::moveToWaypoint(Vector2& wp, bool useLocalization, double velocity, double angularVelocity, double errorRange)
{
  // move to waypoint wp until within the error range
  Vector2 pos;
  double yaw, angle, distance;
  do
  {
    robot.Read();

    // get robot's current position based on either localization or odometry
    if (useLocalization)
    {
      pos = getLocalizedPos();
      yaw = getLocalizedYaw();
    }
    else
    {
      pos = getOdometerPos();
      yaw = getOdometerYaw();
    }
       
    // obtain angle and distance needed to reach the waypoint
    getAngleDistanceToWaypoint(pos, yaw, wp, angle, distance);

    // rotate towards then travel to the given waypoint
    rotateByRadians(angle, angularVelocity);
    moveForwardByMeters(distance, velocity);
   
    // print robot's current position
    std::cout << "\n";
    if (useLocalization) printLocalizedPosition();
    else                 printOdometerPosition();

    // handle any bumper events
    handleBump();
  } while (!hasReachedWaypoint(pos, wp, errorRange));
}

/**
 * The robot will constantly move forward whilst only relying on its laser.
 * It will cease movement upon reaching a dead end.
 *
 * @param forwardVelocity - velocity that the robot moves forward at in m/s
 * @param angularVelocity - angular velocity that the robot rotates at in rad/s
 */ 
void Robot::autoPilotLaser(double forwardVelocity, double angularVelocity)
{
  if (!sp) return;

  double minLeft, minRight;
  TurnDirection::Enum dir;

  while (1)
  {
    printLaserData();

    // get min left and right data from the laser
    minLeft  = sp->MinLeft();
    minRight = sp->MinRight();

    // reached a dead end, stop moving
    if (minLeft < 0.30 && minRight < 0.30) break;

    // steady the robot to the center of its lane
    if      (minLeft < 1.225 && minRight < 1.225) dir = TurnDirection::None;
    else if (minRight < minLeft)                  dir = TurnDirection::Left;
    else if (minRight > minLeft)                  dir = TurnDirection::Right;

    // move robot
    setSpeed(forwardVelocity, angularVelocity, dir);
  }
}
