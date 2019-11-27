/**
 * Proj5
 * Group10: Aguilar, Andrew, Kamel, Kennedy
 */
#include "Robot.h"
#include <fstream>
#include <vector>

// Forward declarations
int  readPlanLength(void);
void readPlan(double*, int);
void printPlan(double*,int);  
void writePlan(double*, int);
std::vector<Vector2> getWaypoints();

int main(int argc, char *argv[])
{  
  // Create robot with lasers enabled and movement+rotation scaled up by 1.35
  Robot robot(true, 1.35, 1.35);

  // Generate waypoints needed to follow the given plan
  std::vector<Vector2> waypoints = getWaypoints();

  // Determine how to handle bumper events
  // SimpleBumper bumperState(TurnDirection::Left);
  AutoPilot bumperState;

  // Follow the plan
  for (int i = 0; i < waypoints.size(); ++i)
  {
    // print where we are heading to
    std::cout << "\nNow moving to coordinate: " << waypoints[i] << "\n";

    // move to given location
    robot.moveToWaypoint(waypoints[i], bumperState, true, 3.0, 1.0, 0.2);

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

/**
 * Generates the waypoints needed to follow the given plan
 * @return Vector of Vector2 waypoints
 */ 
std::vector<Vector2> getWaypoints()
{
  // The set of coordinates that makes up the plan
  int pLength = readPlanLength();     // Find out how long the plan is from plan.txt
  double *plan = new double[pLength]; // Create enough space to store the plan

  readPlan(plan, pLength);  // Read the plan from the file plan.txt.
  printPlan(plan,pLength);  // Print the plan on the screen
  writePlan(plan, pLength); // Write the plan to the file plan-out.txt

  // create waypoints vector
  std::vector<Vector2> vec;
  for (int i = 0; i < pLength; i += 2)
  {
    vec.push_back(Vector2(plan[i], plan[i+1]));
  }

  return vec;
}
