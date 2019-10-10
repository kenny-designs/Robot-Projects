/**
 * Proj4 Part1
 * Group10: Aguilar, Andrew, Kamel, Kennedy
 */

#include "Robot.h"

int main(int argc, char *argv[])
{
  // Initialize our robot with lasers disabled
  Robot robot(false);

  // Enable motors
  robot.setMotorEnable(true);

  Vector2 wp(5.0, -3.5);
  robot.moveToWaypoint(wp, true, 1.0);

  /*
  robot.rotateByRadians(M_PI_4  * 1.35, 1.0);
  robot.moveForwardByMeters(3.0 * 1.35, 1.0);
  robot.rotateByRadians(M_PI_2  * 1.35, 1.0);
  robot.moveForwardByMeters(3.0 * 1.35, 1.0);
  */
}
