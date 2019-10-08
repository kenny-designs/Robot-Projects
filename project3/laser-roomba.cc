#include "Robot.h"

int main(int argc, char *argv[])
{
  // Initialize our robot with isSimulation set to true
  Robot robot;

  // Enable motors
  robot.setMotorEnable(true);

  // testing by moving the robot forward
  robot.moveForwardByMeters(5.0);
}

