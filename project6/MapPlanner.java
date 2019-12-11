import java.util.Scanner;
import java.util.AbstractCollection;
import java.util.ArrayList;
import java.io.*;

public class MapPlanner {
	final static int MAX_SIZE = 32;

	public static void main(String[] args) {

		int[][] oGrid = new int[MAX_SIZE][MAX_SIZE];
		ArrayList<Double> plan = new ArrayList<>();
		
		readMap(oGrid);
		printMap(oGrid);
		writeMap(oGrid);
		
		readPlan(plan);
		System.out.println(plan.toString());
	}

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
		} catch (FileNotFoundException e) {
			System.out.println("File not found. Exiting");
			System.exit(0);
			e.printStackTrace();
		}
	}

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

	static // Print the map on the screen
	void printMap(int[][] oMap) {
		for (int i = MAX_SIZE - 1; i >= 0; i--) {
			for (int j = 0; j < MAX_SIZE; j++) {
				System.out.print(oMap[i][j] + " ");
			}
			System.out.println();
		}
	}

	void readPlanLength() {

	}

	static ArrayList<Double> readPlan(ArrayList<Double> plan) {
	File planFile = new File("plan.txt");
	try {
		Scanner in = new Scanner(planFile);
		in.next(); // Skip first value
		while(in.hasNext()) {
			plan.add(in.nextDouble());
		}
		return plan;
	} catch (FileNotFoundException e) {
		System.out.println("File not found! Exiting!");
		System.exit(0);
	}
	return null;
	}

	static void printPlan() {

	}

	static void writePlan() {

	}
}