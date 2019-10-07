#include <iostream>
#include <cstdlib>
#include <libplayerc++/playerc++.h>
#include <stdint.h>

using namespace PlayerCc;

// forward declarations
void printBlob(player_blobfinder_blob& blob);

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
  player_blobfinder_blob_t  myBlob;     // A single blob
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
      printBlob(bf.GetBlob(i));
    }

    std::cout << "Speed:     " << speed    << "\n";
    std::cout << "Turn rate: " << turnrate << "\n\n";

    // For safety, in the absence of a controller, do nothing.
    speed = 0;
    turnrate = 0;
    pp.SetSpeed(speed, turnrate);
  }
}


void printBlob(player_blobfinder_blob& blob)
{
  std::cout << "Id: "    << blob.id           << "\n";
  std::cout << "Color: " << (short)blob.color << "\n";
  std::cout << "Area: "  << blob.area         << "\n";
  std::cout << "X: "     << blob.x            << "\n";
  std::cout << "Y: "     << blob.y            << "\n";
  std::cout << " ";
}
