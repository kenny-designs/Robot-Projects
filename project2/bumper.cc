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
  Waypoint wp {1, 2};

  // Movement
  robot.moveForwardByMeters(0.1);
  //robot.rotateByRadians(M_PI_4);

  robot.printPosition();
}

