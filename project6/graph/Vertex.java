/**
 * Vertex that holds a reference to each vertex above, below,
 * to the left, and to the right of it.
 */ 
public class Vertex
{
  public boolean isOccupied;
  public boolean wasVisited;
  public int pathNum;
  public Vertex[] adjVerts;
  public int x, y;

  public Vertex(boolean isOccupied, int x, int y)
  {
    this.isOccupied = isOccupied;
    this.x = x;
    this.y = y;
    wasVisited = false;
    pathNum = 0;
    adjVerts = new Vertex[4];
  }

  /**
   * Connect the vertex to its neighbors
   */ 
  public void setNeighbors(Vertex top, Vertex right, Vertex bottom, Vertex left) {
    adjVerts[0] = top;
    adjVerts[1] = right;
    adjVerts[2] = bottom;
    adjVerts[3] = left;
  }

  public String toString() {
    return isOccupied ? "1" : Integer.toString(pathNum);
  }

  public void printNeighbors() {
    System.out.printf("Top: %d, Right: %d, Bottom: %d, Left: %d\n",
        adjVerts[0] != null ? adjVerts[0].pathNum : -1,
        adjVerts[1] != null ? adjVerts[1].pathNum : -1,
        adjVerts[2] != null ? adjVerts[2].pathNum : -1,
        adjVerts[3] != null ? adjVerts[3].pathNum : -1
    );
  }
}
