# Data for the iRobot roomba, provided with Player 2.1.1
#
# Modfied by Simon Parsons

define roomba position
(
  size [0.33 0.33]

  # this polygon approximates the circular shape of a Roomba
  polygons 1
  polygon[0].points 16
  polygon[0].point[0] [ 0.225 0.000 ]
  polygon[0].point[1] [ 0.208 0.086 ]
  polygon[0].point[2] [ 0.159 0.159 ]
  polygon[0].point[3] [ 0.086 0.208 ]
  polygon[0].point[4] [ 0.000 0.225 ]
  polygon[0].point[5] [ -0.086 0.208 ]
  polygon[0].point[6] [ -0.159 0.159 ]
  polygon[0].point[7] [ -0.208 0.086 ]
  polygon[0].point[8] [ -0.225 0.000 ]
  polygon[0].point[9] [ -0.208 -0.086 ]
  polygon[0].point[10] [ -0.159 -0.159 ]
  polygon[0].point[11] [ -0.086 -0.208 ]
  polygon[0].point[12] [ -0.000 -0.225 ]
  polygon[0].point[13] [ 0.086 -0.208 ]
  polygon[0].point[14] [ 0.159 -0.159 ]
  polygon[0].point[15] [ 0.208 -0.086 ]

  # this bumper array VERY crudely approximates the Roomba's bumpers
  bumper( bcount 2  
	  blength 0.33
          bpose[0] [0.12  0.12  45]
          bpose[1] [0.12 -0.12 -45] 
        )

  # SP additions, based on the .inc file for the Pioneer etc.
  #
  # draw a nose on the robot so we can see which way it points
  gui_nose 1
  #
  # differential steering model
  drive "diff"

)
