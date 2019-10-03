#include "Robot.h"
#include "Vector2.h"

int main(int argc, char *argv[])
{  
  // Initialize our robot with isSimulation set to true
  Robot robot;

  // Enable motors
  robot.setMotorEnable(true);

  // testing the robots ability to always go back to the origin
  // FEEL FREE TO PLAY WITH THIS TO SEE HOW THE ROBOT REACTS TO
  // BEING MOVED AROUND!
  Vector2 origin(0, 0);
  for (;;) { robot.moveToWaypoint(origin); }
}

