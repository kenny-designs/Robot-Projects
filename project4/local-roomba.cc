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
  //robot.moveToWaypoint(wp, true);
  robot.rotateByRadians(0.78);
  robot.moveForwardByMeters(1.0);
}
