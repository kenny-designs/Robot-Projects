#include "Robot.h"
#include "Vector2.h" // struct that holds x and y coordinates
#include <vector>    // holding a series of waypoints

// forward declarations
void wallWorld(Robot& robot);
bool wallWorldStopCondition(Robot* robot);
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
 * the center of the world and end up where it started. In particular,
 * the robot will do this by strictly using its bumpers.
 *
 * @param robot - the robot that will be solving the level
 */ 
void wallWorld(Robot& robot)
{
  // guide the robot around the blob in a counter-clockwise direction
  robot.autoPilot(wallWorldStopCondition, TurnDirection::Left);

  // robot finished moving around the blob, go back to starting point
  std::cout << "Returning to origin.\n\n";
  Vector2 origin(0,0);
  robot.moveToWaypoint(origin);
}


/**
 * Passed as a function pointer to the robot's autoPilot method
 * to determine when to stop movement. In particular, the robot
 * will stop auto pilot movement upon entering the rectangular space
 * between the points (-2, 5) and (2, 4)
 *
 * @param robot - The robot we are checking if meets the stop condition
 */ 
bool wallWorldStopCondition(Robot* robot)
{
  double x = robot->getXPos(),
         y = robot->getYPos();

  if (x > -2.0 && x < 2.0 && y > 4.0 && y < 5.0)
  {
    std::cout << "Robot is now within the rectangular space between points\n" <<
                 "(-2,5) and (2,4). Stop condition has been reached. Ceasing auto-pilot.\n\n";
    return true;
  }

  return false;
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
