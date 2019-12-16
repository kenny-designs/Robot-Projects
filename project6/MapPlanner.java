import java.util.Scanner;
import java.util.ArrayList;
import java.awt.Dimension;
import java.io.*;

public class MapPlanner {
    final static int MAX_SIZE = 32;

    public static void main(String[] args) {
        //General Declarations
        int[][] oGrid = new int[MAX_SIZE][MAX_SIZE];
        ArrayList<Double> plan = new ArrayList<>();

        //Map Operations
        readMap(oGrid);
        printMap(oGrid);
        writeMap(oGrid);

        //Plan Operations
        readPlan(plan);
        //printPlan(plan);
        writePlan(plan);

        // feeding into wavefront
        try {
          Wavefront wf = new Wavefront(oGrid);
          //wf.wavefrontPass();
          wf.autoRun();
          //ArrayList<Dimension> finalPath = wf.getPath();
        }
        catch (Exception e)
        {
          System.out.println(e);
          e.printStackTrace();
        }
    }

    /**
     * This method takes in a 2D array and updates it with the values of the given
     * file
     *
     * @param oMap -- Occupancy Map
     */
    static // Read a map in from the file map.txt
    void readMap(int[][] oMap) {
        File mapFile = new File("map.txt");
        try {
            Scanner in = new Scanner(mapFile);
            for (int i = MAX_SIZE - 1; i >= 0; i--) {
                for (int j = 0; j < MAX_SIZE; j++) {
                    oMap[i][j] = in.nextInt();
                }

            }
            in.close();
        } catch (FileNotFoundException e) {
            System.out.println("File not found. Exiting");
            e.printStackTrace();
            System.exit(0);
        }
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
     * Takes in a 2D array and iterates through the values and prints to the console
     *
     * @param oMap - Occupancy Map
     */
    static // Print the map on the screen
    void printMap(int[][] oMap) {
        for (int i = MAX_SIZE - 1; i >= 0; i--) {
            for (int j = 0; j < MAX_SIZE; j++) {
                System.out.print(oMap[i][j] + " ");
            }
            System.out.println();
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

    /***
     * Takes the ArrayList of coordinates and prints to the console.
     *
     * @param plan -- ArrayList of coordinates
     */
    static void printPlan(ArrayList<Double> plan) {
        System.out.println();
        System.out.print("   x     y\n");
        for (int i = 0; i < plan.size(); i++) {
            System.out.printf("%5.1f ", plan.get(i));
            if ((i > 0) && ((i % 2) != 0)) {
                System.out.println();
            }
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
