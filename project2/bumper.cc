// mathematical constants to help with rotating the robot in radians e.g. M_PI
#define _USE_MATH_DEFINES

#include <cmath>
#include "Robot.h"

int main(int argc, char *argv[])
{  
  // Initialize our robot with isSimulation set to true
  Robot robot;

  // Enable motors
  robot.setMotorEnable(true);

  robot.moveForwardByMeters(1.0);
  robot.printPosition();

  /*
  // Control loop
  while(true) 
    {    
      double turnrate, speed;

      // Read from the proxies.
      robot.Read();

      // What does odometry tell us? In other words, how far do we
      // think we have gone?
      std::cout << "x: " << pp.GetXPos()  << std::endl;
      std::cout << "y: " << pp.GetYPos()  << std::endl;
      std::cout << "a: " << pp.GetYaw()  << std::endl;


      // Print out what the bumpers tell us:
      std::cout << "Left  bumper: " << bp[0] << std::endl;
      std::cout << "Right bumper: " << bp[1] << std::endl;
      
      // If either bumper is pressed, go backwards. Depending on the
      // combination of bumpers that are pressed, turn some also,
      // trying to turn away from the point of contact. 
      //
      // Otherwise just go forwards
      if(bp[0] || bp[1]){

	speed=-0.5;

	// Left bumper in contact, right bumper not in contact. Turn
	// to the right.  
	//
	// dtor converts from degrees to radians.
	if (bp[0] && !bp[1]) {  
	  turnrate=dtor(-10);  
	}

	// Left bumper not in contact, right bumper in contact. Turn
	// to the left
	if (!bp[0] && bp[1]) {
	  turnrate=dtor(10);
	}

	// Both left and right bumpers in contact. Make a random
	// choice about which way to turn.
	if (bp[0] && bp[1]) {
	  if(rand()%2 > 0){
	    turnrate=dtor(-10);
	  }
	  else {
	    turnrate=dtor(10);
	  }
	}
      } 
      else {
	  turnrate = 0;   
	  speed=0.5;
	}     

      // What did we decide to do?
      std::cout << "Speed: " << speed << std::endl;      
      std::cout << "Turn rate: " << turnrate << std::endl << std::endl;

      // Send the motion commands that we decided on to the robot.
      pp.SetSpeed(speed, turnrate);  
    }
*/
}


