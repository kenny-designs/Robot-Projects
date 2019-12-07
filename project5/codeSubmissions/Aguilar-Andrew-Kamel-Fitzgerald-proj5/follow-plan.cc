/**
 * Proj5
 * Group10: Aguilar, Andrew, Kamel, Fitzgerald
 */
#include "Robot.h"
#include <fstream>
#include <vector>

#define PLAN_INPUT_FILE_NAME  "plan.txt"     // file that we are reading the plan from
#define PLAN_OUTPUT_FILE_NAME "plan-out.txt" // file that we are writing the plan to

// Forward declarations
int  readPlanLength();
void readPlan (double* plan, int length);
void printPlan(double* plan, int length);  
void writePlan(double* plan, int length);
void followPlan(std::vector<Vector2>& waypoints, Robot& robot);
std::vector<Vector2> getWaypoints();

int main(int argc, char *argv[])
{  
  // Create robot with lasers enabled and movement+rotation scaled up by 1.35
  Robot robot(true, 1.35, 1.35);

  // Generate waypoints needed to follow the given plan
  std::vector<Vector2> waypoints = getWaypoints();

  // follow the generated plan
  followPlan(waypoints, robot);
}

/**
 * Opens the PLAN_INPUT_FILE_NAME and reads the first element, which should be
 * an even integer, and returns it.
 */
int readPlanLength()
{
  int length;

  std::ifstream planFile;
  planFile.open(PLAN_INPUT_FILE_NAME);

  planFile >> length;
  planFile.close();

  // Some minimal error checking
  if((length % 2) != 0)
  {
    std::cout << "The plan has mismatched x and y coordinates" << std::endl;
    exit(1);
  }

  return length;
}

/**
 * Given the number of coordinates, read them in from
 * PLAN_INPUT_FILE_NAME and put them in the array plan.
 */
void readPlan(double* plan, int length)
{
  int skip;

  std::ifstream planFile;
  planFile.open(PLAN_INPUT_FILE_NAME);

  planFile >> skip;
  for(int i = 0; i < length; i++)
  {
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
  for(int i = 0; i < length; i++)
  {
    std::cout.width(5);
    std::cout << plan[i] << " ";
    if((i > 0) && ((i % 2) != 0))
    {
      std::cout << std::endl;
    }
  }
  std::cout << std::endl;
}

/**
 * Send the plan to the file PLAN_OUTPUT_FILE_NAME, preceeded by
 * the information about how long it is.
 */
void writePlan(double* plan, int length)
{
  std::ofstream planFile;
  planFile.open(PLAN_OUTPUT_FILE_NAME);

  planFile << length << " ";
  for(int i = 0; i < length; i++)
  {
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
  int pLength = readPlanLength();     // Find out how long the plan
  double *plan = new double[pLength]; // Create enough space to store the plan

  readPlan (plan, pLength);  // Read the plan
  printPlan(plan, pLength);  // Print the plan on the screen
  writePlan(plan, pLength);  // Write the plan

  // create waypoints vector
  std::vector<Vector2> vec;
  for (int i = 0; i < pLength; i += 2)
  {
    vec.push_back(Vector2(plan[i], plan[i+1]));
  }

  return vec;
}

/**
 * Has the robot follow a given series of waypoints
 *
 * @param waypoints - vector of waypoints for the robot to follow
 * @param robot     - the robot that will be following the waypoints
 */ 
void followPlan(std::vector<Vector2>& waypoints, Robot& robot)
{
  // Determine how to handle bumper events
  AutoPilot bumperState;

  // Follow the plan
  for (int i = 0; i < waypoints.size(); ++i)
  {
    // print where we are heading to
    std::cout << "\nNow moving to coordinate: " << waypoints[i] << "\n";

    // move to given location
    robot.moveToWaypoint(waypoints[i], bumperState, 3.0, 1.0, 0.3);

    // report the robot's actual final location
    std::cout << "Now at the following position:\n";
    robot.printLocalizedPosition();
  }
}
