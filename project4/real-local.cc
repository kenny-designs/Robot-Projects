/**
 * Proj4 Part2
 * Group10: Aguilar, Andrew, Kamel, Kennedy
 */

#include <iostream>
#include <libplayerc++/playerc++.h>
using namespace PlayerCc;  

// Forward declarations
player_pose2d_t readPosition(LocalizeProxy& lp);
void printLaserData(LaserProxy& sp);
void printRobotData(BumperProxy& bp, player_pose2d_t pose);

int main(int argc, char *argv[])
{  
  // Variables
  int counter = 0;
  double speed;            // How fast do we want the robot to go forwards?
  double turnrate;         // How fast do we want the robot to turn?
  player_pose2d_t pose;    // For handling localization data
  player_laser_data laser; // For handling laser data

  // Proxies
  PlayerClient    robot("localhost");  
  BumperProxy     bp(&robot,0);  
  Position2dProxy pp(&robot,0);
  LocalizeProxy   lp (&robot, 0);
  LaserProxy      sp (&robot, 0);

  // Enable motor
  pp.SetMotorEnable(true);

  // Main control loop
  while(true) 
  {    
    // Update information from the robot.
    robot.Read();

    // Read new information about position
    pose = readPosition(lp);

    // Print information about the laser
    if(counter > 2)
    {
      printLaserData(sp);
    }

    // Print data on the robot to the terminal
    printRobotData(bp, pose);

    // If either bumper is pressed, stop. Otherwise just go forwards
    if(bp[0] || bp[1])
    {
      speed    = 0;
      turnrate = 0;
    } 
    else {
      speed    = 0.1;
      turnrate = 0;
    }     

    // What are we doing?
    std::cout << "Speed: " << speed << std::endl;      
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
 * The localization proxy gives us a set of "hypotheses", each of
 * which is a number of possible locations for the robot, and from
 * each we extract the mean, which is a pose.
 *
 * As the number of hypotheses drops, the robot should be more sure
 * of where it is.
 */

player_pose2d_t readPosition(LocalizeProxy& lp)
{
  player_localize_hypoth_t hypothesis;
  player_pose2d_t          pose;
  uint32_t                 hCount;
  double                   weight;

  // Need some messing around to avoid a crash when the proxy is starting up.
  hCount = lp.GetHypothCount();

  std::cout << "AMCL gives us " << hCount + 1 
    << " possible locations:" << std::endl;

  if (hCount > 0)
  {
    for (int i = 0; i <= hCount; i++)
    {
      hypothesis = lp.GetHypoth(i);
      pose       = hypothesis.mean;
      weight     = hypothesis.alpha;
      std::cout << "X: " << pose.px << "\t";
      std::cout << "Y: " << pose.py << "\t";
      std::cout << "A: " << pose.pa << "\t";
      std::cout << "W: " << weight  << std::endl;
    }
  }
  // Returns the mean of the last hypothesis, it isn't necessarily the right one.
  return pose;
}

void printLaserData(LaserProxy& sp)
{
  double maxRange, minLeft, minRight, range, bearing;
  int points;

  maxRange  = sp.GetMaxRange();
  minLeft   = sp.MinLeft();
  minRight  = sp.MinRight();
  range     = sp.GetRange(5);
  bearing   = sp.GetBearing(5);
  points    = sp.GetCount();

  //Print out useful laser data
  std::cout << "Laser says..." << std::endl;
  std::cout << "Maximum distance I can see: " << maxRange << std::endl;
  std::cout << "Number of readings I return: " << points << std::endl;
  std::cout << "Closest thing on left: " << minLeft << std::endl;
  std::cout << "Closest thing on right: " << minRight << std::endl;
  std::cout << "Range of a single point: " << range << std::endl;
  std::cout << "Bearing of a single point: " << bearing << std::endl;
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
