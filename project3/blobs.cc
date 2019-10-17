#include <iostream>
#include <cstdlib>
#include <libplayerc++/playerc++.h>
#include <stdint.h>

using namespace PlayerCc;

// definitions
#define LEFT_X   135
#define RIGHT_X  185
#define MAX_AREA 4500

// forward declarations
bool isRedBlob(player_blobfinder_blob_t& blob);
bool isFacingBeacon(player_blobfinder_blob_t& blob);
void printBlob(player_blobfinder_blob_t& blob);
double getAngularVelocityToBeacon(player_blobfinder_blob_t& blob);
double getVelocityToBeacon(player_blobfinder_blob_t& blob);

int main(int argc, char *argv[])
{ 
  // Set up proxies
  PlayerClient    robot("localhost");  
  BumperProxy     bp(&robot,0);  
  Position2dProxy pp(&robot,0);
  PlayerClient    blobfinder("localhost");
  BlobfinderProxy bf(&blobfinder,0);

  // Variables
  player_blobfinder_blob_t  blob;
  double velocity, angularVelocity;

  // Allow the program to take charge of the motors (take care now)
  pp.SetMotorEnable(true);

  // Control loop
  while(true) 
  {
    // Read from the proxies
    robot.Read();
    blobfinder.Read();

    // Stop movement if a bumper is pressed
    if (bp.IsAnyBumped())
    {
      pp.SetSpeed(0, 0);
      continue;
    }

    // reset velocity and angularVelocity to 0
    velocity        = 0;
    angularVelocity = 0;

    // Iterate over blobs if any. Only process the first red blob we see
    for (int i = 0; i < bf.GetCount(); i++)
    {
      // skip any blobs that aren't red
      blob = bf.GetBlob(i);
      if (!isRedBlob(blob)) continue;

      // otherwise, we have a red blob! print to screen
      printBlob(blob);

      // if not facing the beacon, rotate towards it
      if (!isFacingBeacon(blob))
      {
        angularVelocity = getAngularVelocityToBeacon(blob);
      }
      // otherwise, begin moving towards it
      else
      {
        velocity = getVelocityToBeacon(blob);
      }

      // only need to process a single red blob, break
      break;
    }

    // apply calculated velocity and angularVelocity to the robot
    std::cout << "Velocity:        " << velocity        << "\n" <<
                 "AngularVelocity: " << angularVelocity << "\n\n";
    pp.SetSpeed(velocity, angularVelocity);
  }
}

/**
 * Returns true if we are dealing with a red blob
 *
 * @param blob - the blob we wish to check the color of
 * @return     - true if the color of the blob is red
 */
bool isRedBlob(player_blobfinder_blob_t& blob)
{
  return ((short)blob.color) == 0;
}

/**
 * Returns true if we are currently facing the beacon
 *
 * @param blob - the blob that we are checking if centered on the screen
 * @return     - true if facing the beacon. i.e. the blob
 */
bool isFacingBeacon(player_blobfinder_blob_t& blob)
{
  return blob.x >= LEFT_X && blob.x <= RIGHT_X;
}

/**
 * Print blob data
 *
 * @param blob - the blob that we wish to print the data of
 */
void printBlob(player_blobfinder_blob_t& blob)
{
  std::cout << "Id:    " << blob.id           << "\n" <<
               "Color: " << (short)blob.color << "\n" <<
               "Area:  " << blob.area         << "\n" <<
               "X:     " << blob.x            << "\n" <<
               "Y:     " << blob.y            << "\n\n";
}

/**
 * Returns the appropriate angular velocity needed to face the beacon
 *
 * @param blob - the blob, or beacon, that the robot should face
 * @return     - the angular velocity in rad/s the robot must rotate to face the beacon
 */
double getAngularVelocityToBeacon(player_blobfinder_blob_t& blob)
{
  if (isFacingBeacon(blob)) return 0;

  // looking too far left, right turn
  if (blob.x < LEFT_X)
  {
    return 0.05;
  }

  // otherwise, the robot is looking too far right. Turn left
  return -0.05;
}

/**
 * Returns the appropriate velocity needed to reach the beacon
 *
 * @param blob - the blob, or beacon, the robot must approach within two feet
 * @return     - the velocity the robot should travel to visit the beacon
 */
double getVelocityToBeacon(player_blobfinder_blob_t& blob)
{
  // if already within 2ft, stop
  if (blob.area >= MAX_AREA) return 0;

  // left default velocity is 0.1 m/s because it should adjust using the algorithm below
  double vel = 0.1;

  // scale velocity based on distance between the robot and beacon
  /*From Yasmine: I think that the previous algorithm: vel *= blob.area/MAX_Area
  was actually reducing the speed when further away from our beacon, because the blob.area 
  value is smaller the further the beacon is, therefore we were dividing a large number (max area = 4500)
  into a small number and getting a small decimal factor to multiply into our 0.1/s velocity,
  giving us a slower velocity. For example if the blob.area is 1000 (so much farther away than 2ft), 
  then our velocity would be 0.1 (0.2222) which 0.02/sec velocity. I think this was leading us to 
  evoke the 0.05 min speed condition previously set below. 

  So I’ve switched them around, now the blob.area divides into the Max_Area, 
  I left the default/initial speed at 0.1 because it’s going to adjust anyway 
  (as I understand it) and I’ve set the max speed cap as 0.5/s and min speed cap of 0.1/s.
  Hard to say what this will look like in real life, but hopefully will be easy to adjust
  if necessary.
  */
  vel *=  MAX_AREA/blob.area ;

  // cap velocity between 0.1 m/s and 0.5 m/s
  if      (vel < 0.1) vel = 0.1;
  else if (vel > 0.5)  vel = 0.5;

  return vel;
}

