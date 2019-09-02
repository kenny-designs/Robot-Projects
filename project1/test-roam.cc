// include mathematical constants e.g. PI
#define _USE_MATH_DEFINES

#include <cmath>
#include "Robot.h"

// forward declarations
double feetToMeter(double);
void drawSquare(Robot&);
void writeHi(Robot&);

int main(int argc, char *argv[])
{
  // Initialize our robot
  Robot robot;

  // Enable motors
  robot.setMotorEnable(true);

  // Draw a 3ft by 3ft square with the robot
  drawSquare(robot);
}

// helper method to convert feet to meters
double feetToMeter(double feet) { return feet / 3.2808; }


void drawSquare(Robot& robot)
{
  // Move in a 3ft by 3ft square shape
  double squareSideLength = feetToMeter(3.0);
  for (int i = 0; i < 4; ++i)
  {
    robot.moveForwardByMeters(squareSideLength);  // move forward 3 feet
    robot.rotateByRadians(-M_PI_2);               // rotate clockwise by PI/2 radians (90 degrees)
  }
}

void writeHi(Robot& robot)
{
}
