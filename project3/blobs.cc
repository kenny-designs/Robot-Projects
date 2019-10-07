/*
 *  blobs.cc
 * 
 *  The beginnings of a controller that uses the blobfinder.
 *
 *  Created:     Simon Parsons
 *  Date:        7th October 2011
 *  
 */


#include <iostream>
#include <cstdlib>
#include <libplayerc++/playerc++.h>
#include <stdint.h>

int main(int argc, char *argv[])
{  
  using namespace PlayerCc;  

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

  // Allow the program to take charge of the motors (take care now)
  pp.SetMotorEnable(true);


  // Control loop
  while(true) 
    {    
      double turnrate, speed;

      // Read from the proxies
      robot.Read();
      blobfinder.Read();

      // We only want to drive if the bumpers are not pressed

      if ((bp[0] == 0)&&(bp[1] == 0))
	{
	  if (bf.GetCount() > 0){

	    // Here the blobfinder has found some color blobs

	    std::cout << std::endl;
	    std::cout << bf.GetCount() << " Blobs found!" << std::endl;
	
	    for(int i = 0; i < bf.GetCount(); i++){
	      std::cout << "Id: "    << bf.GetBlob(i).id    << std::endl;
	      std::cout << "Color: " << (short)bf.GetBlob(i).color << std::endl;
	      std::cout << "Area: "  << bf.GetBlob(i).area  << std::endl;
	      std::cout << "X: "     << bf.GetBlob(i).x     << std::endl;
	      std::cout << "Y: "     << bf.GetBlob(i).y     << std::endl;
	      std::cout << std::endl;

	    }
	  }
	  else
	    {
	      // Here the blobfinder doesn;t see any blobs

	    }
	}
      
      // If bumpers are pressed, do nothing.

      else{
	speed = 0;
	turnrate = 0;
      }

      // What are we doing?
      //std::cout << pp << std::endl;      
      std::cout << "Speed: " << speed << std::endl;      
      std::cout << "Turn rate: " << turnrate << std::endl << std::endl;
      
      // For safety, in the absence of a controller, do nothing.
      speed = 0;
      turnrate = 0;
      pp.SetSpeed(speed, turnrate);
  
    } // end of while(true) loop
}


