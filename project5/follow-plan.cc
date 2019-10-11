/**
 * Proj5
 * Group10: Aguilar, Andrew, Kamel, Kennedy
 */
#include "Robot.h"
#include <fstream>

// Forward declarations
int  readPlanLength(void);
void readPlan(double*, int);
void printPlan(double*,int);  
void writePlan(double*, int);

int main(int argc, char *argv[])
{  
  // Create robot with lasers enabled and movement+rotation scaled up by 1.35
  Robot robot(true, 1.35, 1.35);

  // The set of coordinates that makes up the plan
  int pLength;
  double *plan;

  // Plan handling
  // 
  // A plan is an integer, n, followed by n doubles (n has to be
  // even). The first and second doubles are the initial x and y
  // (respectively) coordinates of the robot, the third and fourth
  // doubles give the first location that the robot should move to, and
  // so on. The last pair of doubles give the point at which the robot
  // should stop.
  pLength = readPlanLength(); // Find out how long the plan is from plan.txt
  plan = new double[pLength]; // Create enough space to store the plan
  readPlan(plan, pLength);    // Read the plan from the file plan.txt.
  printPlan(plan,pLength);    // Print the plan on the screen
  writePlan(plan, pLength);   // Write the plan to the file plan-out.txt

  // TODO: this is a bit crude. But it works
  HandleBumpConfig bumpConfig(TurnDirection::Left); // if both bumpers pressed, turn left
  for (int i = 0; i < pLength; i += 2)
  {
    // obtain the next step in our master plan
    Vector2 wp(plan[i], plan[i+1]);

    // print where we are heading to
    std::cout << "\nNow moving to coordinate: " << wp << "\n";

    // move to given location
    robot.moveToWaypoint(wp, true, 3.0, 1.0, 0.5, bumpConfig);

    // report the robot's actual final location
    std::cout << "Now at the following position:\n";
    robot.printLocalizedPosition();
  }
}

/**
 * Open the file plan.txt and read the first element, which should be
 * an even integer, and return it.
 */
int readPlanLength(void)
{
  int length;

  std::ifstream planFile;
  planFile.open("plan.txt");

  planFile >> length;
  planFile.close();

  // Some minimal error checking
  if((length % 2) != 0){
    std::cout << "The plan has mismatched x and y coordinates" << std::endl;
    exit(1);
  }

  return length;
}

/**
 * Given the number of coordinates, read them in from plan.txt and put
 * them in the array plan.
 */
void readPlan(double* plan, int length)
{
  int skip;

  std::ifstream planFile;
  planFile.open("plan.txt");

  planFile >> skip;
  for(int i = 0; i < length; i++){
    planFile >> plan[i];
  }

  planFile.close();
}

/**
 * Print the plan on the screen, two coordinates to a line, x then y
 * with a header to remind us which is which.
 */
void printPlan(double* plan, int length)
{
  std::cout << std::endl;
  std::cout << "   x     y" << std::endl;
  for(int i = 0; i < length; i++){
    std::cout.width(5);
    std::cout << plan[i] << " ";
    if((i > 0) && ((i % 2) != 0)){
      std::cout << std::endl;
    }
  }
  std::cout << std::endl;
}


/**
 * Send the plan to the file plan-out.txt, preceeded by the information
 * about how long it is.
 */
void writePlan(double* plan, int length)
{
  std::ofstream planFile;
  planFile.open("plan-out.txt");

  planFile << length << " ";
  for(int i = 0; i < length; i++){
    planFile << plan[i] << " ";
  }

  planFile.close();
}
