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

  Vector2 wp(5.0, -3.5);
  robot.moveToWaypoint(wp, true);

  //player_pose2d_t pose = robot.getPoseFromLocalizeProxy();

  /*
  while (true)
  {
    printRobotData(robot.getPoseFromLocalizeProxy());
    robot.setSpeed(1.0, 0);
  }
  */
}

/**
 * Print out data on the state of the bumpers and the current location
 * of the robot.
 */
void printRobotData(player_pose2d_t pose)
{
  std::cout << "We are at"      << "\n" <<
               "X: " << pose.px << "\n" <<
               "Y: " << pose.py << "\n" <<
               "A: " << pose.pa << "\n\n";
}
