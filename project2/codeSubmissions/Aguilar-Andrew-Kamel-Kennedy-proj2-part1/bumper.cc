/**
 * Proj2 Part1
 * Group10: Aguilar, Andrew, Kamel, Kennedy
 */

#include "Robot.h"
#include "Vector2.h" // struct that holds x and y coordinates
#include <vector>    // holding a series of waypoints

// forward declarations
void wallWorld(Robot& robot);
void mazeWorld(Robot& robot);
void traverseWaypoints(std::vector<Vector2>& waypoints, Robot& robot);

int main(int argc, char *argv[])
{  
  // Initialize our robot with isSimulation set to true
  Robot robot;

  // Enable motors
  robot.setMotorEnable(true);

  // Traverse wall world
  wallWorld(robot);

  // Traverse maze world
  // mazeWorld(robot);
}

/**
 * The given robot will complete a circuit around the blob in
 * the center of the world and end up where it started.
 *
 * @param robot - the robot that will be solving the level
 */ 
void wallWorld(Robot& robot)
{
  // define how the robot should handle bumper presses
  HandleBumpConfig leftConf(TurnDirection::Left, TurnDirection::Left, TurnDirection::Left),
                   rightConf(TurnDirection::Right, TurnDirection::Right, TurnDirection::Right);
  double x, y;
  while (1)
  {
    // get the robot's current position
    x = robot.getXPos(),
    y = robot.getYPos();

    // break upon reentering quadrant in which we started from    
    if (x > -2.0 && x < 2.0 && y > 4.0 && y < 5.0) break;

    // handle bumper depending on if the robot is in the center of the map or not
    robot.handleBump((x < 11.0 && x > 0 && y < 11.0 && y > 0) ? rightConf : leftConf, M_PI_4, 0.5, 1.0);

    // advance forward at 1m/s
    robot.setSpeed(1.0, 0);
  }

  // cicuit has been completed, go back to origin
  std::cout << "Returning to origin.\n\n";
  Vector2 origin(0,0);
  robot.moveToWaypoint(origin, 1.0);
}

/**
 * Generates waypoints for the robot to follow to solve the
 * maze.png level. In particular, it will traverse the maze from
 * the bottom left-hand corner to the very top left-hand corner.
 *
 * @param robot - the robot that will be solving the level
 */ 
void mazeWorld(Robot& robot)
{
  // vector to hold all waypoints for the robot to travel to
  std::vector<Vector2> waypoints;

  // populate vector with waypoints
  waypoints.push_back(Vector2( 0,  0));
  waypoints.push_back(Vector2(12,  1));
  waypoints.push_back(Vector2(12,  4));
  waypoints.push_back(Vector2( 1,  4));
  waypoints.push_back(Vector2( 1,  8));
  waypoints.push_back(Vector2(12,  8));
  waypoints.push_back(Vector2(12, 12));
  waypoints.push_back(Vector2( 0, 12));

  // have robot travel to each waypoint
  traverseWaypoints(waypoints, robot);
}

/**
 * Makes the given robot travel along a series of Vector2 waypoints
 * starting from the first element and ending with the last.
 *
 * @param waypoints - a series of waypoints for the robot to travel to
 * @param robot     - the robot that will be traveling to each waypoint
 */
void traverseWaypoints(std::vector<Vector2>& waypoints, Robot& robot)
{
  for (int i = 0; i < waypoints.size(); i++)
  {
    robot.moveToWaypoint(waypoints[i]);
  }
}
