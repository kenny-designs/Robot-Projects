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

  // track and current and previous positions of the robot
  Vector2 cur(0, 0), prev(-1.0, -1.0);

  // true if the robot is centered on the map, false if along the edges.
  bool isCentered = false;
  while (1)
  {
    // get the robot's current position
    cur.x = robot.getXPos(),
    cur.y = robot.getYPos();

    // break upon reentering the initial quadrant
    if (cur.x > -2.0 && cur.x < 2.0 && cur.y > 4.0 && cur.y < 5.0) break;

    // check if the robot is in the center of the map or along the edges
    isCentered = cur.x < 11.0 && cur.x > 0 && cur.y < 11.0 && cur.y > 0;

    // handle bumper presses
    robot.handleBump(isCentered ? rightConf : leftConf, M_PI_4, 0.5, 1.0);

    // handle the rare case in which the bumpers do not trigger despite the robot being stuck
    if (cur == prev) robot.setSpeed(-1.0, 1.0, isCentered ? TurnDirection::Right : TurnDirection::Left);

    // regular movement at 1m/s
    robot.setSpeed(1.0, 0);

    // set current location to now be the  previous location.
    prev.x = cur.x;
    prev.y = cur.y;
  }

  // cicuit has been completed, go back to origin with a single waypoint
  std::cout << "The circuit has been completed! Returning to origin.\n";
  Vector2 origin(0,0);
  robot.moveToWaypoint(origin, 1.0);
  std::cout << "It is done.\n";
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
