import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.Queue;
import java.util.LinkedList;

/**
 * Holds a 2D graph of vertices to represent a square grid
 */ 
public class Graph {
  private final int MAX_VERTS;   // maximum number of vertices in the graph
  private final int SIDE_LENGTH; // length of one side of the graph
  private Vertex vertexList[];   // array of vertices

  /**
   * Creates a new 2D grid graph data-structure
   *
   * @param sideLength - The length of one side of the graph
   */ 
  Graph(int sideLength, String mapFileName) {
    // setup the max and current number of vertices
    SIDE_LENGTH = sideLength;
    MAX_VERTS   = SIDE_LENGTH * SIDE_LENGTH;

    // create arrays for both vertices and their adjacency matrix
    vertexList = new Vertex[MAX_VERTS];

    // read in our vertex data
    readMap(mapFileName);
  }

  /**
   * Reads in map from a given *.txt file
   */
  void readMap(String mapFileName) {
    try {
      // create the file we wish to read in
      File mapFile = new File(mapFileName);
      Scanner in   = new Scanner(mapFile);

      // populate the vertexList with the vertices
      for (int i = 0; i < MAX_VERTS; i++) {
        vertexList[i] = new Vertex((in.nextInt() == 1));
      }

      // connect vertices in as you would a 2D grid
      Vertex top, right, bottom, left;
      for (int i = 0; i < MAX_VERTS; i++) {
        vertexList[i].setNeighbors(
            i - SIDE_LENGTH < 0 ? null : vertexList[i - SIDE_LENGTH],                // top
            i + 1 >= SIDE_LENGTH * (i / SIDE_LENGTH + 1) ? null : vertexList[i + 1], // right
            i + SIDE_LENGTH >= MAX_VERTS ? null : vertexList[i + SIDE_LENGTH],       // bottom
            i - 1 <= SIDE_LENGTH * (i / SIDE_LENGTH) ? null : vertexList[i - 1]      // left
        );
      }

      in.close();
    }
    catch (FileNotFoundException e) {
      System.out.println("File not found. Exiting");
      e.printStackTrace();
      System.exit(0);
    }
  }

  /**
   * Obtains the path between 2 points
   *
   * @param startX - x coord of the starting location
   * @param startY - y coord of the starting location
   * @param goalX  - x coord of the end location
   * @param goalY  - y coord of the end location
   */ 
  public void getPath(int startX, int startY, int goalX, int goalY) {
    // TODO: account for negative coords for the actual robot
    int startIndex = startX + SIDE_LENGTH * startY;
    int goalIndex  = goalX + SIDE_LENGTH * goalY;

    // creating path
    Queue<Vertex> queue = new LinkedList<>();
    vertexList[goalIndex].wasVisited = true;
    queue.add(vertexList[goalIndex]);
    boolean isFound = false;
    while(!queue.isEmpty() && !isFound) {
      Vertex top = queue.poll();
      for (int i = 0; i < top.adjVerts.length; i++) {
        Vertex v = top.adjVerts[i];
        if (v == null || v.isOccupied || v.wasVisited) {
          continue;
        }
        else {
          v.pathNum = top.pathNum + 1;
          v.wasVisited = true;
          if (v == vertexList[startIndex]) {
            isFound = true;
            break;
          }

          queue.add(v);
        }
      }
    }

    // now unwind
    Vertex cur = vertexList[startIndex];
    while (cur.pathNum != 0) {
      /*
      printMap();
      try { System.in.read(); } catch(Exception e) {}
      System.out.println("Current is: " + cur);
      cur.printNeighbors();
      */

      for (Vertex v : cur.adjVerts) {
        if (v != null && v.pathNum == cur.pathNum - 1) {
          cur.pathNum = -5;
          cur = v;
          break;
        }
      }
    }
  }

  /**
   * Prints the graph to the console
   */
  public void printMap() {
    for (int i = 0; i < MAX_VERTS; i++) {
      if (i % SIDE_LENGTH == 0) System.out.println();
      System.out.printf("%2d ", vertexList[i].isOccupied ? 1 : vertexList[i].pathNum);
    }
    System.out.println();
  }

  /**
   * Dilates all occupied vertices
   */ 
  public void dilate() {
    LinkedList<Vertex> occupiedVerts = new LinkedList<>();
    for (Vertex v : vertexList) {
      if (v.isOccupied) occupiedVerts.add(v);
    }

    for (Vertex parentVert : occupiedVerts) {
      for (Vertex childVert : parentVert.adjVerts) {
        if (childVert != null) {
          childVert.isOccupied = true;
          childVert.pathNum = 1;
        }
      }
    }
  }
}
