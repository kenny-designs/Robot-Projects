import java.util.Scanner;
import java.util.ArrayList;
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
    LinkedList<Point2D.Double> plan = graph.getPath(11, 28, 29, 3);
    graph.printMap();

    // print the final plan
    printPlan(plan);
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
   * Opens the file "plan.txt" and returns the first integer.
   *
   * @return pLength - Plan Length (How many Coordinates)
   */
  static int getPlanLength() {
    File planFile = new File("plan.txt");
    try {
      Scanner in = new Scanner(planFile);
      int pLength = in.nextInt();
      in.close();
      return pLength;
    } catch (FileNotFoundException e) {
      System.out.println("File Not Found! Exiting!");
      e.printStackTrace();
      System.exit(0);
    }
    return 0;
  }

  /**
   * Takes an ArrayList of doubles from the file plan.txt
   * <p>
   * Since the first value is not a cordinate it is skipped with in.next() before
   * the loop begins
   *
   * @param plan -- An ArrayList containing the doubles for the cordinates
   * @return plan | Null
   */
  static ArrayList<Double> readPlan(ArrayList<Double> plan) {
    File planFile = new File("plan.txt");
    try {
      Scanner in = new Scanner(planFile);
      in.next(); // Skip first value
      while (in.hasNext()) {
        plan.add(in.nextDouble());
      }
      in.close();
      return plan;
    } catch (FileNotFoundException e) {
      System.out.println("File not found! Exiting!");
      e.printStackTrace();
      System.exit(0);
    }
    return null;
  }

  /**
   * Takes a LinkedList of coordinates and prints to the console.
   *
   * @param plan -- LinkedList of coordinates
   */
  static void printPlan(LinkedList<Point2D.Double> plan) {
    System.out.println();
    System.out.print("    x     y\n");

    for (Point2D.Double pt : plan) {
      System.out.printf("%5.1f %5.1f\n", pt.x, pt.y);
    }
    System.out.println();
  }

  /**
   * Takes the ArrayList of type double and writes how many coordinates first,
   * followed by the coordinates themselves.
   * @param plan -- ArrayList of coordinates
   */
  static void writePlan(ArrayList<Double> plan) {
    File planFile = new File("plan-out.txt");
    try {
      PrintWriter printer = new PrintWriter(planFile);

      printer.print(getPlanLength() + " ");
      for (int i = 0; i < getPlanLength(); i++) {

        printer.print(plan.get(i) + " ");
      }

      printer.close();
    } catch (

        FileNotFoundException e) {
      // TODO Auto-generated catch block
      e.printStackTrace();
        }
  }
}
