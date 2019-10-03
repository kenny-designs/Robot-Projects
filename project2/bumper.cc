// mathematical constants to help with rotating the robot in radians e.g. M_PI
#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>  // dynamic array
#include "Robot.h"

int main(int argc, char *argv[])
{  
  // Initialize our robot with isSimulation set to true
  Robot robot;

  // Enable motors
  robot.setMotorEnable(true);

  // Make waypoints
  /*
  std::vector<Vector2> waypoints;
  waypoints.push_back(Vector2(0, 0));
  waypoints.push_back(Vector2(1, 0));
  waypoints.push_back(Vector2(1, 1));
  waypoints.push_back(Vector2(0, 1));
  waypoints.push_back(Vector2(0, -1));
  waypoints.push_back(Vector2(-1, -1));
  waypoints.push_back(Vector2(-1, 2));
  waypoints.push_back(Vector2(2, 2));
  waypoints.push_back(Vector2(2, -1));
  waypoints.push_back(Vector2(0, -1));
  waypoints.push_back(Vector2(0, 0));

  // Move to said waypoints
  for (int i = 0; i < waypoints.size(); ++i)
  {
    robot.moveToWaypoint(waypoints[i]);
  }
  */

  // testing the robots ability to always go back to the origin
  // FEEL FREE TO PLAY WITH THIS TO SEE HOW THE ROBOT REACTS TO
  // BEING MOVED AROUND!
  Vector2 origin(0, 0);
  for (;;) { robot.moveToWaypoint(origin); }
}

