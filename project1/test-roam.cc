#include "Robot.h"

int main(int argc, char *argv[])
{
  // Initialize our robot
  Robot robot;

  // Enable motors
  robot.setMotorEnable(true);

  // move in a square fashion
  robot.moveForwardOverTicks(0.1, 50);
  robot.rotateOverTicks(0.1, 157);

  robot.moveForwardOverTicks(0.1, 50);
  robot.rotateOverTicks(0.1, 157);

  robot.moveForwardOverTicks(0.1, 50);
  robot.rotateOverTicks(0.1, 157);

  robot.moveForwardOverTicks(0.1, 50);
  robot.rotateOverTicks(0.1, 157);
}
