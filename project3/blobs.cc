#include <iostream>
#include <cstdlib>
#include <libplayerc++/playerc++.h>
#include <stdint.h>

using namespace PlayerCc;

// centering the beacon on the screen
#define WIDTH         320
#define CENTER_X      160
#define CENTER_OFFSET 25

// angular velocity
#define MAX_ROT_SPEED 0.1
#define MIN_ROT_SPEED 0.05

// velocity
#define MAX_AREA  4500
#define MAX_SPEED 0.5
#define MIN_SPEED 0.1

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

    velocity        = 0;             // robot isn't moving by default
    angularVelocity = MAX_ROT_SPEED; // robot scanning for a beacon by default

    // Iterate over blobs if any. Only process the first red blob we see
    for (int i = 0; i < bf.GetCount(); i++)
    {
      // look for a red blob
      blob = bf.GetBlob(i);
      if (!isRedBlob(blob)) continue;

      // print blob info to the screen
      printBlob(blob);

      // center beacon to the camera's view
      if (!isFacingBeacon(blob))
      {
        angularVelocity = getAngularVelocityToBeacon(blob);
      }
      // if beacon is centered, begin moving towards it and prevent default beacon search
      else
      {
        velocity = getVelocityToBeacon(blob);
        angularVelocity = 0;
      }

      // only process the first red blob encountered, break
      break;
    }

    // apply calculated velocity and angularVelocity to the robot
    std::cout << "Velocity:        " << velocity        << "\n" <<
                 "AngularVelocity: " << angularVelocity << "\n\n";

    // approach the beacon
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
  return blob.x >= CENTER_X - CENTER_OFFSET &&
         blob.x <= CENTER_X + CENTER_OFFSET;
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

  // set x to a value between 0 to CENTER_X
  double x = blob.x;
  if (x > CENTER_X)
  {
    x = WIDTH - x;
  }

  // scale angular velocity depending on how far away we are from the center
  double angularVelocity = (x / CENTER_X) * (MIN_ROT_SPEED - MAX_ROT_SPEED) + MAX_ROT_SPEED;

  // rotate in the correct direction
  return blob.x > CENTER_X ? -angularVelocity : angularVelocity;
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

  // default velocity is MIN_SPEED because it should adjust using the algorithm below
  double vel = MIN_SPEED;

  // scale velocity based on distance between the robot and beacon
  vel *=  MAX_AREA / blob.area ;

  // cap velocity between the MIN_SPEED and MAX_SPEED
  if      (vel < MIN_SPEED) vel = MIN_SPEED;
  else if (vel > MAX_SPEED) vel = MAX_SPEED;

  return vel;
}

