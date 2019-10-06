#include "Robot.h"
#include "Vector2.h"

int main(int argc, char *argv[])
{  
  // Initialize our robot with isSimulation set to true
  Robot robot;

  // Enable motors
  robot.setMotorEnable(true);

  // testing the robots ability to always go back to the origin
  // Vector2 origin(0, 0);
  // for (;;) { robot.moveToWaypoint(origin); }

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
}