/**
 * Proj6
 * Group10: Aguilar, Andrew, Kamel, Fitzgerald
 */
#include "Robot.h"
#include <fstream>
#include <vector>

#define PLAN_INPUT_FILE_NAME  "plan-out.txt" // file that we are reading the plan from
#define MAP_INPUT_FILE_NAME   "map-out.txt"  // file that we are reading the map from

const int SIZE = 32; // The number of squares per side of the occupancy grid
                     // (which we assume to be square)

// Forward declarations
void readMap(int[SIZE][SIZE]);
void printMap(int [SIZE][SIZE]);

int  readPlanLength();
void readPlan (double* plan, int length);
void printPlan(double* plan, int length);  
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
  mapFile.open(MAP_INPUT_FILE_NAME);

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
 * Generates the waypoints needed to follow the given plan
 * @return Vector of Vector2 waypoints
 */ 
std::vector<Vector2> getWaypoints()
{
  // The occupancy grid
  int oGrid[SIZE][SIZE];

  // Print the map to the console
  readMap(oGrid);
  printMap(oGrid);

  // The set of coordinates that makes up the plan
  int pLength = readPlanLength();     // Find out how long the plan
  double *plan = new double[pLength]; // Create enough space to store the plan

  readPlan (plan, pLength);  // Read the plan
  printPlan(plan, pLength);  // Print the plan on the screen

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
    robot.moveToWaypoint(waypoints[i], bumperState, 3.0, 1.0, 0.2);

    // report the robot's actual final location
    std::cout << "Now at the following position:\n";
    robot.printLocalizedPosition();
  }
}
