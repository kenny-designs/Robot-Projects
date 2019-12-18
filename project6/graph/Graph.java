import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.io.PrintWriter;
import java.util.Queue;
import java.util.LinkedList;
import java.awt.geom.Point2D; // 2D points for waypoints

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
   * @param sideLength  - The length of one side of the graph
   * @param mapFileName - Name of the file to load in for the map
   */ 
  Graph(int sideLength, String mapFileName) {
    // set default values
    SIDE_LENGTH = sideLength;
    MAX_VERTS   = SIDE_LENGTH * SIDE_LENGTH;

    // create an array to hold all of our vertices
    vertexList = new Vertex[MAX_VERTS];

    // read in our map vertex data
    readMap(mapFileName);

    // connect all vertices
    connectVertices();
  }

  /**
   * Reset all vertices in the grid
   */
  private void resetAllVertices() {
    for (Vertex v : vertexList) {
      Vertex.reset(v);
    }
  }

  /**
   * Iterates over the vertexList and creates edges between all vertices and their
   * adjacent neighbors.
   */ 
  private void connectVertices() {
    // alias for SIDE_LENGTH to aid readability
    int sl = SIDE_LENGTH;

    // connect each vertex to its adjacent neighbors
    for (int i = 0; i < MAX_VERTS; i++) {
      vertexList[i].setNeighbors(
          i - sl < 0                  ? null : vertexList[i - sl], // top
          i + 1  >= sl * (i / sl + 1) ? null : vertexList[i + 1],  // right
          i + sl >= MAX_VERTS         ? null : vertexList[i + sl], // bottom
          i - 1  <= sl * (i / sl)     ? null : vertexList[i - 1]   // left
      );
    }
  }

  /**
   * Dilates all occupied vertices.
   */ 
  private void dilateOccupiedVertices() {
    // create a list of all occupied vertices
    LinkedList<Vertex> occupiedVerts = new LinkedList<>();
    for (Vertex v : vertexList) {
      if (v.isOccupied) occupiedVerts.add(v);
    }

    // dilate each occupiedVert
    for (Vertex occupiedVert : occupiedVerts) {  
      Vertex.dilate(occupiedVert);
    }
  }

  /**
   * Marks the path between a start and goal point.
   *
   * @param startIndex - the index of the starting vertex
   * @param goalIndex  - the index of the goal vertex
   * @return true if a path can be made. False otherwise.
   */ 
  private boolean markPathWavefront(int startIndex, int goalIndex) {
    // create a queue to allow us to explore vertices one level at a time
    Queue<Vertex> queue = new LinkedList<>();

    // add the goal to the queue and mark was visited
    vertexList[goalIndex].wasVisited = true;
    queue.add(vertexList[goalIndex]);

    // search for the starting vertex until we find it or run out of vertices
    boolean isFound = false;
    while(!queue.isEmpty() && !isFound) {
      // dequeue the front vertex of the queue
      Vertex front = queue.poll();

      // mark each neighbor of the front vertex until we find the starting one
      for (Vertex v : front.adjVerts) {
        // skip the vertex it doesn't exist, is occupied, or was already visited
        if (v == null || v.isOccupied || v.wasVisited) continue;

        // mark the vertex's order for the wavefront algorithm and tag it as visited
        v.pathNum = front.pathNum + 1;
        v.wasVisited = true;

        // check if this is our starting index, break if so
        if (v == vertexList[startIndex]) {
          isFound = true;
          break;
        }

        // enqueue the vertex so we can check its neighbors too
        queue.add(v);
      }
    }

    return isFound;
  }

  /**
   * Follows the path created by markPathWavefront() and returns the 
   * generated waypoints.
   *
   * @param startIndex - the index of the vertex we are starting from
   * @param goalIndex  - the index of the vertex we are heading towards
   * @return LinkedList of the waypoints for the robot to follow
   */ 
  private LinkedList<Vertex> generateWavefrontWaypoints(int startIndex,
                                                        int goalIndex) {
    // create a list of waypoints for the robot to follow
    LinkedList<Vertex> waypoints = new LinkedList<>();

    // set the current vertex to the the starting one
    Vertex cur = vertexList[startIndex];

    // unwind from the starting vertex until we run into the goal
    int lastDir = -1;
    while (cur.pathNum != 0) {
      // look for adjacent vertex that comes before the current one
      for (int i = 0; i < cur.adjVerts.length; i++) {
        // check for adjacent vertex that comes before the current one
        if (cur.adjVerts[i] != null && cur.adjVerts[i].pathNum == cur.pathNum - 1) {

          // TODO: remove. Only used for visualizing
          cur.pathNum = -5;

          // if we change directions, add to waypoints
          if (i != lastDir) {
            lastDir = i;
            waypoints.add(cur);
          }

          // since we found the next waypoint, set it to be the current one
          cur = cur.adjVerts[i];
          break;
        }
      }
    }

    // add the final point
    waypoints.add(vertexList[goalIndex]);

    return waypoints;
  }

  /**
   * Obtains the path between 2 points
   *
   * @param start - coord of the starting location
   * @param goal  - coord of the end location
   * @return list of vertices
   */ 
  public LinkedList<Vertex> getWaypoints(Point2D.Double start,
                                                 Point2D.Double goal) {
    // convert given coords to a scale of 0 to SIDE_LENGTH
    convertToSingleQuad(start);
    convertToSingleQuad(goal);

    // find the start and end indices
    int startIndex = (int)start.x + SIDE_LENGTH * (int)start.y;
    int goalIndex  = (int) goal.x + SIDE_LENGTH * (int) goal.y;

    // reset all vertices before making the path
    resetAllVertices();

    // dilate all vertices to account for walls
    dilateOccupiedVertices();

    // mark the path between the start and goal points via the wavefront algorithm
    boolean isPathPossible = markPathWavefront(startIndex, goalIndex);

    // if a path is not possible, return null and warn the user
    if (!isPathPossible) {
      System.out.printf(
          "ERROR! Cannot find path between points (%.1f, %.1f) and (%.1f %.1f).\n",
          start.x, start.y, goal.x, goal.y
      );

      resetAllVertices();
      return null;
    }

    // obtain a list of vertices for the robot to follow
    LinkedList<Vertex> waypoints = generateWavefrontWaypoints(startIndex, goalIndex);
    resetAllVertices();

    // mark waypoints on the map
    for (Vertex v : waypoints) {
      v.pathNum = 2;
    }

    return waypoints;
  }

  /**
   * Converts a given Point2D.Double four quadrant point to a single quadrant one
   * that the Graph can use
   */
  public void convertToSingleQuad(Point2D.Double pt) {
    double sl = (double)SIDE_LENGTH;
    double sl_2 = sl / 2.0;

    pt.x = 2.0 * pt.x + sl_2;
    pt.y = sl - (2.0 * pt.y + sl_2);
  }

  /**
   * Reads in map from a given *.txt file
   *
   * @param mapFileName - Name of the file to load in for the map
   */
  void readMap(String mapFileName) {
    try {
      // create the file we wish to read in
      File mapFile = new File(mapFileName);
      Scanner in   = new Scanner(mapFile);

      // declare variables to help with vertex creation
      double sl_4 = (double)(SIDE_LENGTH) / 4.0;
      double x, y;
      boolean isOccupied;

      // populate the vertexList with vertices
      for (int i = 0; i < MAX_VERTS; i++) {
        // determine if the new vertex is occupied and what its coords are
        isOccupied = in.nextInt() == 1;
        x          = (double)(i % SIDE_LENGTH) / 2.0 - sl_4;
        y          = sl_4 - (double)(i / SIDE_LENGTH) / 2.0;

        // add the new vertex to the list
        vertexList[i] = new Vertex(isOccupied, x, y);
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
   * Prints the graph to the console.
   */
  public void printMap() {
    for (int i = 0; i < MAX_VERTS; i++) {
      if (i % SIDE_LENGTH == 0) System.out.println();
      System.out.printf("%2d ", vertexList[i].isOccupied ? 1 : vertexList[i].pathNum);
    }
    System.out.println();
  }

  /**
   * Writes the map to a .txt file called map-out.txt.
   */
  public void writeMap() {
    try {
      // create the file we wish to write out to
      File mapFile = new File("map-out.txt");
      PrintWriter printer = new PrintWriter(mapFile);

      // write the map to the file
      for (int i = 0; i < MAX_VERTS; i++) {
        if (i % SIDE_LENGTH == 0) printer.println();
        printer.print(vertexList[i] + " ");
      }

      // save the file
      printer.close();

    } catch (FileNotFoundException e) {
      System.out.println("Failed to write map to file. Exiting");
      e.printStackTrace();
      System.exit(0);
    }
  }
}
