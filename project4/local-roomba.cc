/**
 * Proj4 Part1
 * Group10: Aguilar, Andrew, Kamel, Kennedy
 */

#include "Robot.h"

int main(int argc, char *argv[])
{
  // Initialize our robot with lasers disabled and movement+rotation scaled up by 1.35
  Robot robot(false, 1.35, 1.35);

  // Enable motors
  robot.setMotorEnable(true);

  Vector2 wp(5.0, -3.5);
  robot.moveToWaypoint(wp, true);

  /*
  robot.rotateByRadians(M_PI_4);
  robot.moveForwardByMeters(3.0);
  */

  // TODO: hard coded values for world42
  /*
  robot.rotateByRadians(M_PI_4 * 1.215);
  robot.moveForwardByMeters(3.0 * 1.215); 
  */
}
