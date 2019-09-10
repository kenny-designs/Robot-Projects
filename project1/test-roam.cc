// include mathematical constants e.g. M_PI
#define _USE_MATH_DEFINES

#include <cmath>
#include "Robot.h"

// forward declarations
double feetToMeter(double);
void drawSquare(Robot&, double, bool);

int main(int argc, char *argv[])
{
  // Initialize our robot
  Robot robot;

  // Enable motors
  robot.setMotorEnable(true);

  // drive forward by 3 feet
  robot.moveForwardByMeters(feetToMeter(3.0));

  // Draw a 3ft by 3ft square with the robot
  // drawSquare(robot, 3.0, true);         // clockwise
  // drawSquare(robot, 3.0, false);        // counter-clockwise
}

// helper method to convert feet to meters
double feetToMeter(double feet) { return feet / 3.2808; }


void drawSquare(Robot& robot, double sideLengthInFeet, bool isClockwise)
{
  // convert feet to meters
  double sideLengthInMeters = feetToMeter(sideLengthInFeet);

  // direction we should make a 90 degree turn
  double turnDirection = isClockwise ? -M_PI_2 : M_PI_2;

  // Move in a 3ft by 3ft square shape
  for (int i = 0; i < 4; ++i)
  {
    robot.moveForwardByMeters(sideLengthInMeters);  // move forward 3 feet
    robot.rotateByRadians(turnDirection);           // rotate clockwise by PI/2 radians (90 degrees)
  }
}
