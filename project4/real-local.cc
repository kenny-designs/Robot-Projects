/**
 * Proj4 Part2
 * Group10: Aguilar, Andrew, Kamel, Kennedy
 */
#include "Robot.h"

int main(int argc, char *argv[])
{
  // Create robot with lasers disabled and movement+rotation scaled up by 1.215
  Robot robot(true, 1.215, 1.215);

  // localize the robot
  for (int i = 0; i < 25; ++i)
  {
    robot.read();
    robot.setSpeed(0, 0.5);
    robot.setSpeed(-1.0, 0);
    printf("Readout #: %d\n", i);
    //robot.printLocalizedPosition();
    robot.getPoseFromLocalizeProxy();
  }

  // Travel to point (5, -3.5)
  Vector2 wp(5.0, -3.5);
  robot.moveToWaypoint(wp, true, 1.0, 1.0);
}
