/**
 * Proj6
 * Group10: Aguilar, Andrew, Kamel, Fitzgerald
 */
#include "Robot.h"
#include <fstream>
#include <vector>

#define PLAN_INPUT_FILE_NAME  "plan-out.txt" // file that we are reading the plan from
#define PLAN_OUTPUT_FILE_NAME "plan-out.txt" // file that we are writing the plan to

const int SIZE = 32; // The number of squares per side of the occupancy grid
                     // (which we assume to be square)

// Forward declarations
void readMap(int[SIZE][SIZE]);
void writeMap(int [SIZE][SIZE]);
void printMap(int [SIZE][SIZE]);

int  readPlanLength();
void readPlan (double* plan, int length);
void printPlan(double* plan, int length);  
void writePlan(double* plan, int length);
void followPlan(std::vector<Vector2>& waypoints, Robot& robot);
std::vector<Vector2> getWaypoints();

int main(int argc, char *argv[])
{  
  // The occupancy grid
  int oGrid[SIZE][SIZE];

  // The set of coordinates that makes up the plan
  int pLength;
  double *plan;

  // Map handling
  //
  // The occupancy grid is a square array of integers, each side of
  // which is SIZE elements, in which each element is either 1 or 0. A
  // 1 indicates the square is occupied, an 0 indicates that it is
  // free space.
  readMap(oGrid);   // Read a map in from the file map.txt
  printMap(oGrid);  // Print the map on the screen
  writeMap(oGrid);  // Write a map out to the file map-out.txt

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
  //writePlan(plan, pLength);   // Write the plan to the file plan-out.txt

  // Create robot with lasers enabled and movement+rotation scaled up by 1.35
  Robot robot(true, 1.35, 1.35);

  // Generate waypoints needed to follow the given plan
  std::vector<Vector2> waypoints = getWaypoints();

  // follow the generated plan
  followPlan(waypoints, robot);
}

/**
 * readMap
 *
 * Reads in the contents of the file map.txt into the array map
 * in such a way that the first element of the last row of the
 * file map.txt is in element [0][0].
 *
 * This means that whatever is in the file looks like the occupancy
 * grid would if you drew it on paper.
 */
void readMap(int map[SIZE][SIZE])
{
  std::ifstream mapFile;
  mapFile.open("map.txt");

  for(int i = SIZE - 1; i >= 0; i--)
  {
    for(int j = 0; j < SIZE; j++)
    {
      mapFile >> map[i][j];
    }
  }

  mapFile.close();
}

/**
 * printMap
 *
 * Print map[][] out on the screen. The first element to be printed
 * is [SIZE][0] so that the result looks the same as the contents of
 * map.txt
 */
void printMap(int map[SIZE][SIZE])
{
  for(int i = SIZE -1; i >= 0; i--)
  {
    for(int j = 0; j < SIZE; j++)
    {
      std::cout << map[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

/**
 * writeMap
 *
 * Write a map into map-out.txt in such a way that the [0][0] element
 * ends up in the bottom left corner of the file (so that the contents
 * of the file look like the relevant occupancy grid.
 */
void writeMap(int map[SIZE][SIZE])
{
  std::ofstream mapFile;
  mapFile.open("map-out.txt");

  for(int i = SIZE - 1; i >= 0; i--)
  {
    for(int j = 0; j < SIZE; j++)
    {
      mapFile << map[i][j];
    }
    mapFile << std::endl;
  }
  mapFile.close();
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
