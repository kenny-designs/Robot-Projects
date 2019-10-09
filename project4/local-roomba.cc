/**
 * Proj4 Part1
 * Group10: Aguilar, Andrew, Kamel, Kennedy
 */

#include "Robot.h"

// Forward declarations
void printRobotData(player_pose2d_t pose);

int main(int argc, char *argv[])
{
  // Initialize our robot with lasers disabled
  Robot robot(false);

  // Enable motors
  robot.setMotorEnable(true);

  player_pose2d_t pose = robot.getPoseFromLocalizeProxy();

  printRobotData(pose);
}

/**
 * Print out data on the state of the bumpers and the current location
 * of the robot.
 */
void printRobotData(player_pose2d_t pose)
{
  // Print out where we are
  std::cout << "We are at" << std::endl;
  std::cout << "X: " << pose.px << std::endl;
  std::cout << "Y: " << pose.py << std::endl;
  std::cout << "A: " << pose.pa << std::endl;
}
