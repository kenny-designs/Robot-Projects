/**
 * Vertex that holds a reference to each vertex above, below,
 * to the left, and to the right of it.
 */ 
public class Vertex {
  public boolean isOccupied;  // marks whether or not the vertex is occupied
  public boolean wasVisited;  // used for pathfinding algorithms
  public boolean wasDilated;  // used to track which nodes were dilated
  public int pathNum;         // used with wavefront algorithm to indicate order
  public Vertex[] adjVerts;   // list of adjacent vertices (top, right, bottom, and left)
  public double x, y;         // location of the vertice in 2D space

  /**
   * Creates a new Vertex.
   * 
   * @param isOccupied - true if the space is vertex is occupied. False otherwise
   * @param x          - x coord of the vertex on a 2D plane
   * @param y          - y coord of the vertex on a 2D plane
   */ 
  public Vertex(boolean isOccupied, double x, double y) {
    this.isOccupied = isOccupied;
    this.x          = x;
    this.y          = y;
    wasVisited      = false;
    wasDilated      = false;
    pathNum         = 0;
    adjVerts        = new Vertex[4];
  }

  /**
   * Connect the vertex to its neighbors.
   *
   * @param top    - reference to the vertex above
   * @param right  - reference to the vertex to the right
   * @param bottom - reference to the vertex below
   * @param left   - reference to the vertex to the left
   */ 
  public void setNeighbors(Vertex top, Vertex right, Vertex bottom, Vertex left) {
    adjVerts[0] = top;
    adjVerts[1] = right;
    adjVerts[2] = bottom;
    adjVerts[3] = left;
  }

  /**
   * Returns vertex data as a string.
   *
   * @return 1 if the vertex is occupied. The pathNum otherwise
   */
  public String toString() {
    return isOccupied ? "1" : Integer.toString(pathNum);
  }

  /**
   * Prints all of the neighbors of the vertex.
   */ 
  public void printNeighbors() {
    System.out.printf("Top: %d, Right: %d, Bottom: %d, Left: %d\n",
        adjVerts[0] != null ? adjVerts[0].pathNum : -1,
        adjVerts[1] != null ? adjVerts[1].pathNum : -1,
        adjVerts[2] != null ? adjVerts[2].pathNum : -1,
        adjVerts[3] != null ? adjVerts[3].pathNum : -1
    );
  }

  /**
   * Resets vertex visited state to false and pathNum to 0 or 1.
   *
   * @param v - vertex to be reset
   */ 
  public static void reset(Vertex v) {
    v.wasVisited = false;

    if (v.wasDilated) {
      v.wasDilated = false;
      v.isOccupied = false;
    }

    v.pathNum = v.isOccupied ? 1 : 0;
  }

  /**
   * Dilates all surrounding vertices similar to minesweeper.
   *
   * 0 0 0    1 1 1
   * 0 1 0 -> 1 1 1
   * 0 0 0    1 1 1
   *
   * @param v - vertex to be dilated
   */ 
  public static void dilate(Vertex v) {
    for (int i = 0; i < v.adjVerts.length; i++) {
      if (v.adjVerts[i] == null) continue;
      dilateSingleVert(v.adjVerts[i]);
      dilateSingleVert(v.adjVerts[i].adjVerts[(i+1)%4]);
    }
  }

  /**
   * Dilates a single vertice.
   *
   * @param v - the vertex to have set dilated
   */ 
  private static void dilateSingleVert(Vertex v) {
    if (v == null || v.isOccupied) return;

    v.isOccupied = true;
    v.wasDilated = true;
    v.pathNum = 1;
  }
}
