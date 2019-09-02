// include mathematical constants e.g. M_PI
#define _USE_MATH_DEFINES

#include <cmath>
#include "Robot.h"

// forward declarations
double feetToMeter(double);
void drawSquare(Robot&, double, bool);
void writeHi(Robot&);

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

  //Write the word 'Hi'
  //writeHi(robot);
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

void writeHi(Robot& robot)
{
  // Draw a capital H
  robot.rotateByRadians(M_PI_2 + M_PI_4);       // rotate 135 degrees to face up
  robot.moveForwardByMeters(2.0);               // travel 2 meters
  robot.rotateByRadians(M_PI);                  // rotate 180 degrees to face down
  robot.moveForwardByMeters(1.0);               // travel 1 meter
  robot.rotateByRadians(M_PI_2);                // rotate 90 degrees to face right
  robot.moveForwardByMeters(1.0);               // travel 1 meter
  robot.rotateByRadians(M_PI_2);                // rotate 90 degrees to face up
  robot.moveForwardByMeters(1.0);               // travel 1 meter
  robot.rotateByRadians(M_PI);                  // rotate 180 degrees to face down
  robot.moveForwardByMeters(2.0);               // travel 2 meters

  // Now draw a lowercase i 
  robot.rotateByRadians(-M_PI_2);               // rotate -90 degrees to face left
  robot.moveForwardByMeters(-1.0);              // travel 1 meter backwards
  robot.rotateByRadians(M_PI_2);                // rotate -90 degrees to face up
  robot.moveForwardByMeters(1.0, -0.1);         // travel 1 meter
  robot.rotateByRadians(M_PI);                  // rotate a 180 degrees
  robot.rotateByRadians(M_PI);                  // rotate a 180 degrees again
}
