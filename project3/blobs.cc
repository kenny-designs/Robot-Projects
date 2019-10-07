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
  std::cout << "Id:    " << blob.id           << "\n" <<
               "Color: " << (short)blob.color << "\n" <<
               "Area:  " << blob.area         << "\n" <<
               "X:     " << blob.x            << "\n" <<
               "Y:     " << blob.y            << "\n\n";
}

/** Returns the appropriate angular velocity needed to face the beacon */
double getAngularVelocityToBeacon(player_blobfinder_blob_t& blob)
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
double getVelocityToBeacon(player_blobfinder_blob_t& blob)
{
  // TODO: check minimum area must be for us to consider the movement complete
  // if already within 2ft, stop
  if (blob.area <= MIN_AREA) return 0;

  // by default, we move at 0.1 m/s
  // TODO: test for the smoothest experience when scaling these values
  double vel = 0.1;

  // adjust to scale with distance from the beacon
  vel *= MIN_AREA / blob.area;

  // if the velocity is now below 0.05m/s, set it to 0.05m/s
  if (vel < 0.05) vel = 0.05;

  return vel;
}

