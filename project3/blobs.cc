#include <iostream>
#include <cstdlib>
#include <libplayerc++/playerc++.h>
#include <stdint.h>

using namespace PlayerCc;

// forward declarations
bool isRedBlob(player_blobfinder_blob_t& blob);
void printBlob(player_blobfinder_blob_t& blob);
void rotateToFaceBeacon(Position2dProxy& pp, player_blobfinder_blob_t& blob);
void moveToBeacon(Position2dProxy& pp, player_blobfinder_blob_t& blob);

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
  double turnrate = 0, speed = 0;

  // Allow the program to take charge of the motors (take care now)
  pp.SetMotorEnable(true);

  // Control loop
  while(true) 
  {
    // Read from the proxies
    robot.Read();
    blobfinder.Read();

    // We only want to drive if the bumpers are not pressed
    if (!(bp[0] || bp[1])) continue;

    // Iterate over blobs if any
    for (int i = 0; i < bf.GetCount(); i++)
    {
      // if a red blob is found, process then break
      player_blobfinder_blob_t blob = bf.GetBlob(i);
      if (isRedBlob(blob))
      {
        printBlob(blob);
        break;
      }
    }

    std::cout << "Speed:     " << speed    << "\n";
    std::cout << "Turn rate: " << turnrate << "\n\n";

    // For safety, in the absence of a controller, do nothing.
    speed = 0;
    turnrate = 0;
    pp.SetSpeed(speed, turnrate);
  }
}

/** Returns true if we are dealing with a red blob */
bool isRedBlob(player_blobfinder_blob_t& blob)
{
  return ((short)blob.color) == 0;
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

/** Rotate to face the beacon */
void rotateToFaceBeacon(Position2dProxy& pp, player_blobfinder_blob_t& blob)
{
  //
}

/** Move to the beacon within 2ft */
void moveToBeacon(Position2dProxy& pp, player_blobfinder_blob_t& blob)
{
  //
}

