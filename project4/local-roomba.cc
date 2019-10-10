/**
 * Proj4 Part1
 * Group10: Aguilar, Andrew, Kamel, Kennedy
 */
#include "Robot.h"

int main(int argc, char *argv[])
{
  // Create robot with lasers disabled and movement+rotation scaled up by 1.35
  Robot robot(false, 1.35, 1.35);

  // Travel to point (5, -3.5)
  Vector2 wp(5.0, -3.5);
  robot.moveToWaypoint(wp, true, 1.0);
}
