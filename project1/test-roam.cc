// include mathematical constants e.g. PI
#define _USE_MATH_DEFINES

#include <cmath>
#include "Robot.h"

// helper method to convert feet to meters
double feetToMeter(double feet) { return feet / 3.2808; }

int main(int argc, char *argv[])
{
  // Initialize our robot
  Robot robot;

  // Enable motors
  robot.setMotorEnable(true);

  // Move in a 3ft by 3ft square shape
  double squareSideLength = feetToMeter(3.0);
  for (int i = 0; i < 4; ++i)
  {
    robot.moveForwardByMeters(squareSideLength);  // move forward 3 feet
    robot.rotateByRadians(-M_PI_2);               // rotate clockwise by PI/2 radians (90 degrees)
  }
}
