import java.util.Scanner;
import java.util.LinkedList;
import java.io.File;
import java.io.PrintWriter;
import java.io.FileNotFoundException;
import java.awt.geom.Point2D;

public class MapPlanner {
  final static int MAX_SIZE = 32;

  public static void main(String[] args) {
    Graph graph = new Graph(MAX_SIZE, "map.txt");

    // create points for the locations are are moving to
    //Point2D.Double start = new Point2D.Double(-6.0, -6.0);
    //Point2D.Double goal  = new Point2D.Double(6.5, 6.5);

    Point2D.Double start = new Point2D.Double(7.0, 7.0);
    Point2D.Double goal  = new Point2D.Double(-6.5, -6.5);


    // create a plan to go from the start to the goal
    LinkedList<Vertex> plan = graph.getWaypoints(start, goal);

    // print and write the map
    graph.printMap();
    graph.writeMap();

    // print and write the final plan
    printPlan(plan);
    writePlan(plan);
  }

  /**
   * Takes a LinkedList of coordinates and prints to the console.
   *
   * @param plan -- LinkedList of coordinates
   */
  static void printPlan(LinkedList<Vertex> plan) {
    System.out.print("\n    x     y\n");

    for (Vertex v : plan) {
      System.out.printf("%5.1f %5.1f\n", v.x, v.y);
    }
  }

  /**
   * Takes a LinkedList of type Point2D.Double and writes how many coordinates first,
   * followed by the coordinates themselves.
   * @param plan -- LinkedList of coordinates
   */
  static void writePlan(LinkedList<Vertex> plan) {
    try {
      File planFile = new File("plan-out.txt");
      PrintWriter printer = new PrintWriter(planFile);

      printer.print((plan.size() * 2) + " ");
      for (Vertex v : plan) {
        printer.print(v.x + " " + v.y + " ");
      }

      printer.close();
    }
    catch (FileNotFoundException e) {
      e.printStackTrace();
    }
  }
}
