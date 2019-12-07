/**
 * Proj6
 * Group10: Aguilar, Andrew, Kamel, Fitzgerald
 */
#include "Robot.h"
#include <fstream>
#include <vector>

const int SIZE = 32; // The number of squares per side of the occupancy grid
                     // (which we assume to be square)

// Forward declarations
void readMap(int[SIZE][SIZE]);
void writeMap(int [SIZE][SIZE]);
void printMap(int [SIZE][SIZE]);
int  readPlanLength(void);
void readPlan(double *, int);
void printPlan(double *,int);  
void writePlan(double *, int);

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
  writePlan(plan, pLength);   // Write the plan to the file plan-out.txt

  // Create robot with lasers enabled and movement+rotation scaled up by 1.35
  Robot robot(true, 1.35, 1.35);

  robot.rotateByRadians(M_PI_4);
  robot.moveForwardByMeters(1.0);
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
 * readPlanLength
 *
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
  if((length % 2) != 0)
  {
    std::cout << "The plan has mismatched x and y coordinates" << std::endl;
    exit(1);
  }

  return length;
}

/**
 * readPlan
 *
 * Given the number of coordinates, read them in from plan.txt and put
 * them in the array plan.
 */
void readPlan(double *plan, int length)
{
  int skip;

  std::ifstream planFile;
  planFile.open("plan.txt");

  planFile >> skip;
  for(int i = 0; i < length; i++)
  {
    planFile >> plan[i];
  }

  planFile.close();
}

/**
 * printPlan
 *
 * Print the plan on the screen, two coordinates to a line, x then y
 * with a header to remind us which is which.
 */
void printPlan(double *plan , int length)
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
 * writePlan
 * 
 * Send the plan to the file plan-out.txt, preceeded by the information
 * about how long it is.
 */
void writePlan(double *plan , int length)
{
  std::ofstream planFile;
  planFile.open("plan-out.txt");

  planFile << length << " ";
  for(int i = 0; i < length; i++)
  {
    planFile << plan[i] << " ";
  }

  planFile.close();
}
