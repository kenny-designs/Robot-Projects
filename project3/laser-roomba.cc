#include "Robot.h"

int main(int argc, char *argv[])
{
  // Initialize our robot with isSimulation set to true
  Robot robot;

  // Enable motors
  robot.setMotorEnable(true);

  // endlessly move forward guided by the laser
  robot.autoPilotLaser();
}

