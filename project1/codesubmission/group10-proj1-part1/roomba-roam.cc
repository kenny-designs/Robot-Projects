/**Proj1 Part1 clockwise: Group10: Aguilar, Andrew, Kamel, Kennedy **/

/** MEASUREMENTS for Clockwise Runs: 
+---------------------------------------------------------------+
|                           Clockwise                           |
+-----+------------------+------------------+-------------------+
| Run | Distance Error X | Distance Error Y | Orientation Error |
+-----+------------------+------------------+-------------------+
|  1  |      5.00 in     |      8.25 in     |   31.027 degrees  |
+-----+------------------+------------------+-------------------+
|  2  |      5.75 in     |      9.50 in     |   31.027 degrees  |
+-----+------------------+------------------+-------------------+
|  3  |      6.00 in     |      8.50 in     |   48.190 degrees  |
+-----+------------------+------------------+-------------------+
|  4  |      7.25 in     |      8.00 in     |   28.072 degrees  |
+-----+------------------+------------------+-------------------+
|  5  |      6.00 in     |      7.00 in     |   27.266 degrees  |
+-----+------------------+------------------+-------------------+
**/

// mathematical constants to help with rotating the robot in radians e.g. M_PI
#define _USE_MATH_DEFINES

#include <cmath>
#include "Robot.h"

// forward declarations
double feetToMeter(double);
void drawSquare(Robot&, double, bool);

int main(int argc, char *argv[])
{
  // Initialize our robot with isSimulation set to true
  // Robot robot;

  // Initialize our robot with isSimulation set to false
  Robot robot(false);

  // Enable motors
  robot.setMotorEnable(true);

  // Draw a 3ft by 3ft square with the robot
  drawSquare(robot, 3.0, true);      // clockwise
  // drawSquare(robot, 3.0, false);  // counter-clockwise
}

/**
 * Helper method to convert feet to meters.
 *
 * @param feet - the distance in feet we wish to convert to meters
 * @return     - the converted distance in meters
 */
double feetToMeter(double feet) { return feet / 3.2808; }

/**
 * Commands the robot to move in a square shape by the given distance
 * in feet and either clockwise or anticlockwise.
 *
 * The Robot class is designed to work best with meters. As such, the
 * given sideLengthInFeet will be converted to match this.
 *
 * @param robot            - the robot that we wish to move in a square shape
 * @param sideLengthInFeet - length of each side of the square in feet
 * @param isClockwise      - true to move the robot clockwise. False for anticlockwise
 */
void drawSquare(Robot& robot, double sideLengthInFeet, bool isClockwise)
{
  // convert feet to meters
  double sideLengthInMeters = feetToMeter(sideLengthInFeet);

  // find the direction we should make a 90 degree turn in
  double turnDirection = isClockwise ? -M_PI_2 : M_PI_2;

  // Move in a 3ft by 3ft square shape
  for (int i = 0; i < 4; ++i)
  {
    robot.moveForwardByMeters(sideLengthInMeters);  // move forward by sideLengthInMeters
    robot.rotateByRadians(turnDirection);           // rotate by PI/2 radians (90 degrees) based on turnDirection
  }
}
