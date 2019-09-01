#include "Robot.h"
#include <cstdlib>
#include <iostream>
#include <string>

/**
 * Set up proxy. Proxies are the datastructures that Player uses to
 * talk to the simulator and the real robot.
 *
 * @param hostname - address to connect to (localhost by default)
 */
Robot::Robot(std::string hostname) : robot(hostname), pp(&robot, 0) {}

/**
 * Enable or disable the robot's motor
 *
 * @param isMotorEnabled - true to enable the motor, false to disable
 */ 
void Robot::setMotorEnable(bool isMotorEnabled)
{
  // Allow the program to take charge of the motors (take care now)
  pp.SetMotorEnable(isMotorEnabled);
}

/**
 * Moves the robot forward with a set velocity over the given number of ticks
 *
 * @param forwardSpeed - velocity of the robot
 * @param ticks - number of ticks to apply the velocity to the robot
 */
void Robot::moveForwardOverTicks(double forwardSpeed, int ticks)
{
  // Enter movement control loop
  for (int curTick = 0; curTick < ticks; ++curTick)
  {
    // Read from the proxies.
    robot.Read();

    std::cout << "Speed: " << forwardSpeed << "\n\n";      

    // Send the motion commands that we decided on to the robot.
    pp.SetSpeed(forwardSpeed, 0);
  }
}

/**
 * Rotates the robot with a set turn rate over the given number of ticks
 *
 * @param turnRate - how quickly the robot should rotate
 * @param ticks - number of ticks to rotate the robot for
 */
void Robot::rotateOverTicks(double turnRate, int ticks)
{
  // Enter movement control loop
  for (int curTick = 0; curTick < ticks; ++curTick)
  {
    // Read from the proxies.
    robot.Read();

    std::cout << "Turn rate: " << turnRate  << "\n";
    std::cout << "Current yaw: " << pp.GetYaw()  << "\n\n";

    // Send the motion commands that we decided on to the robot.
    pp.SetSpeed(0, turnRate);
  }
}
