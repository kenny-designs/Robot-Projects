#include <iostream>
#include <cstdlib>
#include <libplayerc++/playerc++.h>
#include <stdint.h>

using namespace PlayerCc;

// definitions
#define LEFT_X   80
#define RIGHT_X  240
#define MIN_AREA 30

// forward declarations
bool isRedBlob(player_blobfinder_blob_t& blob);
bool isFacingBeacon(player_blobfinder_blob_t& blob);
void printBlob(player_blobfinder_blob_t& blob);
double rotateToFaceBeacon(Position2dProxy& pp, player_blobfinder_blob_t& blob);
double moveToBeacon(Position2dProxy& pp, player_blobfinder_blob_t& blob);

int main(int argc, char *argv[])
{  
  // Set up proxies
  PlayerClient    robot("localhost");  
  BumperProxy     bp(&robot,0);  
  Position2dProxy pp(&robot,0);
  PlayerClient    blobfinder("localhost");
  BlobfinderProxy bf(&blobfinder,0);

  // Variables
  player_blobfinder_data_t  blobList;   // Structure holding the blobs found
  player_blobfinder_blob_t* theBlobs;   // A point to a list of blobs
  double velocity, angularVelocity;

  // Allow the program to take charge of the motors (take care now)
  pp.SetMotorEnable(true);

  // Control loop
  while(true) 
  {
    // for safety, velocity and angularVelocity are 0 by default
    velocity        = 0;
    angularVelocity = 0;

    // Read from the proxies
    robot.Read();
    blobfinder.Read();

    // We only want to drive if the bumpers are not pressed
    if (!(bp[0] || bp[1])) continue;

    // Iterate over blobs if any. Only process the first red blob we see
    for (int i = 0; i < bf.GetCount(); i++)
    {
      // if not a red blob, check the next element
      player_blobfinder_blob_t blob = bf.GetBlob(i);
      if (!isRedBlob(blob)) continue;

      // found a red blob! print it to the screen
      printBlob(blob);

      // if not facing the beacon, rotate towards it
      if (!isFacingBeacon(blob))
      {
        angularVelocity = rotateToFaceBeacon(pp, blob);
      }
      // if the robot is facing the beacon, begin moving towards it
      else
      {
        velocity = moveToBeacon(pp, blob);
      }

      // only need to process a single red blob, break
      break;
    }

    // apply calculated velocity and angularVelocity to the robot
    std::cout << "Velocity:        " << velocity        << "\n";      
    std::cout << "AngularVelocity: " << angularVelocity << "\n\n";
    pp.SetSpeed(velocity, angularVelocity);
  }
}

/** Returns true if we are dealing with a red blob */
bool isRedBlob(player_blobfinder_blob_t& blob)
{
  return ((short)blob.color) == 0;
}

/** Returns true if we are currently facing the beacon */
bool isFacingBeacon(player_blobfinder_blob_t& blob)
{
  // TODO: find what values constitue as 'facing' the beacon
  return blob.x >= LEFT_X && blob.x <= RIGHT_X;
}

/** Print blob data */
void printBlob(player_blobfinder_blob_t& blob)
{
  std::cout << "Id: "    << blob.id           << "\n";
  std::cout << "Color: " << (short)blob.color << "\n";
  std::cout << "Area: "  << blob.area         << "\n";
  std::cout << "X: "     << blob.x            << "\n";
  std::cout << "Y: "     << blob.y            << "\n";
  std::cout << "\n";
}

/** Returns the appropriate angular velocity needed to face the beacon */
double rotateToFaceBeacon(Position2dProxy& pp, player_blobfinder_blob_t& blob)
{
  if (isFacingBeacon(blob)) return 0;

  // TODO: find what values of LEFT_X and RIGHT_X should be
  // looking too far left, right turn
  if (blob.x < LEFT_X)
  {
    return -0.1;
  }

  // otherwise, the robot is looking too far right. Turn left
  return 0.1;
}

/** Returns the appropriate velocity needed to reach the beacon */
double moveToBeacon(Position2dProxy& pp, player_blobfinder_blob_t& blob)
{
  // TODO: check minimum area must be for us to consider the movement complete
  // if already within 2ft, stop
  if (blob.area <= MIN_AREA) return 0;

  // by default, we move at 0.1 m/s
  double vel = 0.1;

  return vel;
}

