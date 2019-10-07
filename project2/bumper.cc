#include "Robot.h"
#include "Vector2.h"
#include <vector>

int main(int argc, char *argv[])
{  
  // Waypoints for World 1
  std::vector<Vector2> waypointsVector1;

  // TODO: we could simplify this into a method for ease of use
  waypointsVector1.push_back(Vector2(0,0));
  waypointsVector1.push_back(Vector2(10,3));
  waypointsVector1.push_back(Vector2(10,6));
  waypointsVector1.push_back(Vector2(9,9));
  waypointsVector1.push_back(Vector2(4,10));
  waypointsVector1.push_back(Vector2(1,6));
  waypointsVector1.push_back(Vector2(0,0));

  // Waypoints for World 2
  std::vector<Vector2> waypointsVector2;

  waypointsVector2.push_back(Vector2(0,0));
  waypointsVector2.push_back(Vector2(12,1));
  waypointsVector2.push_back(Vector2(12,4));
  waypointsVector2.push_back(Vector2(1,4));
  waypointsVector2.push_back(Vector2(1,8));
  waypointsVector2.push_back(Vector2(12,8));
  waypointsVector2.push_back(Vector2(12,12));
  waypointsVector2.push_back(Vector2(0,12));

  // Initialize our robot with isSimulation set to true
  Robot robot;

  // Enable motors
  robot.setMotorEnable(true);

  // TODO: legacy code for manually testing movement. Can be removed
  /*
  // keyboard input
  std::cout << "Greetings human!"                             <<
               "\nI am ready to travel wherever you see fit." <<
               "\nUsing the keyboard, tell me where to go!\n";

  Vector2 wp(0,0);
  for (;;)
  {
    std::cout << "\nEnter x coord: ";
    std::cin >> wp.x;
    std::cout << "Enter y coord: ";
    std::cin >> wp.y;

    std::cout << "\nNow moving to waypoint located at (" << wp.x << ", " << wp.y << ")...\n";
    robot.moveToWaypoint(wp);
    std::cout << "Complete! I am ready for the next destination human.\n";
  }
  */

  // Iterate through World 1 Waypoints
  /*
  for (int i = 0; i < waypointsVector1.size(); i++)
  {
    robot.moveToWaypoint(waypointsVector1[i]);
  }
  */

  // Iterate through World 2 Waypoints
  for (int i = 0; i < waypointsVector2.size(); i++)
  { 
    robot.moveToWaypoint(waypointsVector2[i]);
  }
}

