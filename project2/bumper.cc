// mathematical constants to help with rotating the robot in radians e.g. M_PI
#define _USE_MATH_DEFINES

#include <cmath>
#include "Robot.h"

int main(int argc, char *argv[])
{  
  // Initialize our robot with isSimulation set to true
  Robot robot;

  // Enable motors
  robot.setMotorEnable(true);

  // Make a waypoint
  Waypoint wp1 {1, 1};
  Waypoint wp2 {2, 0};

  // Movement
  robot.moveToWaypoint(wp1);
  robot.moveToWaypoint(wp2);
}

