/**
 * Proj4 Part1
 * Group10: Aguilar, Andrew, Kamel, Kennedy
 */

#include <iostream>
#include <libplayerc++/playerc++.h>
using namespace PlayerCc;  

// Forward declarations
player_pose2d_t readPosition(LocalizeProxy& lp);
void printRobotData(BumperProxy& bp, player_pose2d_t pose);

int main(int argc, char *argv[])
{
  // Variables
  int counter = 0;
  double speed;         // How fast do we want the robot to go forwards?
  double turnrate;      // How fast do we want the robot to turn?
  player_pose2d_t pose; // For handling localization data

  // Proxies
  PlayerClient    robot("localhost");  
  BumperProxy     bp(&robot,0);  
  Position2dProxy pp(&robot,0);
  LocalizeProxy   lp (&robot, 0);

  // Enable motor
  pp.SetMotorEnable(true);

  // Main control loop
  while(true) 
  {    
    // Update information from the robot.
    robot.Read();

    // Read new information about position
    pose = readPosition(lp);

    // Print data on the robot to the terminal
    printRobotData(bp, pose);

    if(bp[0] || bp[1])
    {
      speed    = 0;
      turnrate = 0;
    }
    else
    {
      speed    = 0.1;
      turnrate = 0;
    }     

    // What are we doing?
    std::cout << "Speed:     " << speed << std::endl;      
    std::cout << "Turn rate: " << turnrate << std::endl << std::endl;

    // Send the commands to the robot
    pp.SetSpeed(speed, turnrate);  

    // Count how many times we do this
    counter++;
  }

}


/**
 * Read the position of the robot from the localization proxy. 
 *
 * The localization proxy gives us a hypothesis, and from that we extract
 * the mean, which is a pose. 
 */
player_pose2d_t readPosition(LocalizeProxy& lp)
{
  player_localize_hypoth_t hypothesis;
  player_pose2d_t          pose;
  uint32_t                 hCount;

  // Need some messing around to avoid a crash when the proxy is starting up.
  hCount = lp.GetHypothCount();

  if(hCount > 0)
  {
    hypothesis = lp.GetHypoth(0);
    pose       = hypothesis.mean;
  }

  return pose;
}


/**
 * Print out data on the state of the bumpers and the current location
 * of the robot.
 */
void printRobotData(BumperProxy& bp, player_pose2d_t pose)
{
  // Print out what the bumpers tell us:
  std::cout << "Left  bumper: " << bp[0] << std::endl;
  std::cout << "Right bumper: " << bp[1] << std::endl;

  // Print out where we are
  std::cout << "We are at" << std::endl;
  std::cout << "X: " << pose.px << std::endl;
  std::cout << "Y: " << pose.py << std::endl;
  std::cout << "A: " << pose.pa << std::endl;
}
