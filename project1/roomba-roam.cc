/*
 *  roomba-roam.cc
 * 
 *  Sample code for a robot that is suitable for use with the Roomba
 *  and Create.
 * 
 *  Based on an example provided by Monica Anderson and Jeff Forbes,
 *  via Carlos Jaramillo, and changed to (hopefully) make it easier to
 *  understand.
 *
 *  Modified:    Simon Parsons
 *  Date:        15th June 2009
 *  Last change: 20th September 2011
 *  
 */

#include <iostream>
#include <cstdlib>
#include <libplayerc++/playerc++.h>

int main(int argc, char *argv[])
{  
  using namespace PlayerCc;

  // Set up proxy. Proxies are the datastructures that Player uses to
  // talk to the simulator and the real robot.

  PlayerClient    robot("localhost");  
  Position2dProxy pp(&robot,0);       // The 2D proxy is used to send 
                                      // motion commands.

  int timer = 0;                      // A crude way to time what we do
                                      // is to count.

  // Allow the program to take charge of the motors (take care now)
  pp.SetMotorEnable(true);

  // Control loop
  while(true) 
    {    
      double turnrate, speed;

      // Increment the counter.

      timer++;

      // Read from the proxies.
      robot.Read();

      // First make the rbot go straight ahead, then make it turn, and 
      // finally make it stop.
      if(timer < 30){
	speed = 0.1;
	turnrate = 0;
      }
      else
      if((timer >= 30) && (timer < 60)){
	speed = 0;
	turnrate = 0.1;
      }
      else{
	speed = 0;
	turnrate = 0;
      }
      
      // Print out what we decided to do?
      std::cout << "Speed: " << speed << std::endl;      
      std::cout << "Turn rate: " << turnrate << std::endl << std::endl;

      // Send the motion commands that we decided on to the robot.
      pp.SetSpeed(speed, turnrate);  
    }
  
}


