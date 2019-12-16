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
    graph.dilate();

    // create points for the locations are are moving to
    Point2D.Double start = new Point2D.Double(-2.5, -6.0);
    Point2D.Double goal  = new Point2D.Double(6.5, 6.5);

    // create a plan to go from the start to the goal
    LinkedList<Point2D.Double> plan = graph.getPath(start, goal);

    // print the map
    graph.printMap();

    // print the final plan
    printPlan(plan);
    writePlan(plan);
  }

  /**
   * Take a 2D array as a parameter and writes the new map to a file named
   * "map-out.txt"
   *
   * @param oMap -- Occupancy Map
   */
  static // Write a map out to the file map-out.txt
    void writeMap(int[][] oMap) {
      File mapFile = new File("map-out.txt");
      try {
        PrintWriter printer = new PrintWriter(mapFile);

        for (int i = MAX_SIZE - 1; i >= 0; i--) {
          for (int j = 0; j < MAX_SIZE; j++) {
            printer.print(oMap[i][j] + " ");
          }
          printer.println();
        }

        printer.close();

      } catch (FileNotFoundException e) {
        // Should this ever be caught?
        // Probably not?
        e.printStackTrace();
      }
    }

  /**
   * Takes a LinkedList of coordinates and prints to the console.
   *
   * @param plan -- LinkedList of coordinates
   */
  static void printPlan(LinkedList<Point2D.Double> plan) {
    System.out.print("\n    x     y\n");

    for (Point2D.Double pt : plan) {
      System.out.printf("%5.1f %5.1f\n", pt.x, pt.y);
    }
  }

  /**
   * Takes a LinkedList of type Point2D.Double and writes how many coordinates first,
   * followed by the coordinates themselves.
   * @param plan -- LinkedList of coordinates
   */
  static void writePlan(LinkedList<Point2D.Double> plan) {
    try {
      File planFile = new File("plan-out.txt");
      PrintWriter printer = new PrintWriter(planFile);

      printer.print((plan.size() * 2) + " ");
      for (Point2D.Double pt : plan) {
        printer.print(pt.x + " " + pt.y + " ");
      }

      printer.close();
    }
    catch (FileNotFoundException e) {
      e.printStackTrace();
    }
  }
}
