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
  Waypoint wp {-0.866, -0.5};

  // Movement
  robot.moveToWaypoint(wp);
}

