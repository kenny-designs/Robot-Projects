/**
 * Proj4 Part2
 * Group10: Aguilar, Andrew, Kamel, Kennedy
 */
#include "Robot.h"

// Forward declarations
bool attemptToLocalize(Robot& robot, Vector2& approximatePos, double errorRange);

int main(int argc, char *argv[])
{
  // Create robot with lasers enabled and movement+rotation scaled up by 1.215
  Robot robot(true, 1.215, 1.215);

  // Attempt to localize the robot
  Vector2 approxPos(-6.0, -6.0);
  bool isSuccessful = attemptToLocalize(robot, approxPos, 1.0);

  // Quit program if localization failed
  if (!isSuccessful) return 0;

  // Report success
  std::cout << "Success Localization!\n";

  // Report where the robot is and with the probability of it being there
  std::cout << "I am at:\n";
  robot.printLocalizedPosition();
  std::cout << "I am " << robot.getBestLocalizeHypothesis().alpha << " percent sure of my position.\n";

  // Travel to point (5, -3.5)
  Vector2 wp(5.0, -3.5);
  robot.moveToWaypoint(wp, true, 3.0);

  // Report final destination
  std::cout << "\nSuccess Navigation!\n";
  robot.printLocalizedPosition();
}

/**
 * Localize the robot. Returns true if the robot the robot manages to localize
 * itself approximately where we programmed it to start.
 *
 * @param robot          - the robot we wish to localize
 * @param approximatePos - the assumed starting location of the robot
 * @param errorRange     - the maximum error allowed for localization to still be considered successful
 * @return True if the robot manages to almost guess its position correctly. False otherwise
 */ 
bool attemptToLocalize(Robot& robot, Vector2& approximatePos, double errorRange)
{
  // have the robot move around until it is certain of its position
  robot.localize();

  // get our current position
  Vector2 pos = robot.getLocalizedPos();

  // true if the robot managed to localize itself roughly where it's supposed to be
  return robot.hasReachedWaypoint(pos, approximatePos, errorRange);
}
