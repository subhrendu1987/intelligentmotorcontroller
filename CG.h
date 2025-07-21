#include <stdio.h>  // Required for input/output functions like printf, scanf
#include <stdlib.h> // Required for dynamic memory allocation functions like malloc, free
#include <math.h>   // Required for sin, cos, sqrt, atan2, M_PI

// Define a structure to hold polar coordinates
typedef struct {
    double r;     // Radius
    double theta; // Angle in radians
} PolarCoord;

// Define a structure to hold item data
typedef struct {
    double weight;
    PolarCoord location;
} Item;

/**
 * @brief Calculates the center of gravity for n objects in polar coordinates.
 *
 * This function takes an array of Item structs.
 * It first converts each object's polar coordinates to Cartesian, calculates
 * the overall center of gravity in Cartesian coordinates, and then converts
 * that result back into polar coordinates.
 *
 * @param items     An array of Item structs, each containing weight and polar location.
 * @param n         The number of items.
 * @return          A PolarCoord struct representing the center of gravity (R, Theta).
 * If total weight is zero, it returns {0.0, 0.0}.
 */
PolarCoord calculateCenterOfGravityPolar(const Item items[], int n) {
    double sum_wx = 0.0; // Sum of (weight * x_coordinate)
    double sum_wy = 0.0; // Sum of (weight * y_coordinate)
    double total_weight = 0.0; // Sum of all weights

    // Convert polar to Cartesian and sum weighted coordinates
    for (int i = 0; i < n; i++) {
        double r = items[i].location.r;
        double theta = items[i].location.theta;
        double w = items[i].weight;

        // Convert polar (r, theta) to Cartesian (x, y)
        double x = r * cos(theta);
        double y = r * sin(theta);

        // Add to weighted sums
        sum_wx += w * x;
        sum_wy += w * y;

        // Add to total weight
        total_weight += w;
    }

    PolarCoord cog_polar; // Structure to store the final polar COG

    // Calculate the center of gravity in Cartesian coordinates
    if (total_weight == 0.0) {
        // Handle the case where total weight is zero to avoid division by zero.
        // The center of gravity is undefined in this case.
        fprintf(stderr, "Warning: Total weight is zero. Center of gravity is undefined.\n");
        cog_polar.r = 0.0;
        cog_polar.theta = 0.0;
    } else {
        double X_cog = sum_wx / total_weight;
        double Y_cog = sum_wy / total_weight;

        // Convert Cartesian (X_cog, Y_cog) back to polar (R_cog, Theta_cog)
        cog_polar.r = sqrt(X_cog * X_cog + Y_cog * Y_cog);
        // atan2 is used to correctly determine the angle in all four quadrants.
        cog_polar.theta = atan2(Y_cog, X_cog);
    }

    return cog_polar;
}


/**
 * @brief Draws a character-based plot of items and their center of gravity in a circle.
 *
 * This function creates a square grid representation in the console.
 * It plots a circle, individual item locations, and the center of gravity.
 *
 * @param items      Array of Item structs to be plotted.
 * @param n          Number of items.
 * @param cog        The calculated center of gravity in polar coordinates.
 * @param max_radius The maximum radius among all items and the COG, used for scaling the plot.
 */
void drawPolarPlot(const Item items[], int n, PolarCoord cog, double max_radius) {
    // Define grid dimensions for console output
    const int GRID_WIDTH = 60;
    const int GRID_HEIGHT = 30; // Use a rectangular grid for better aspect ratio in console

    // Dynamically allocate memory for the grid
    char **grid = (char **)malloc(GRID_HEIGHT * sizeof(char *));
    if (grid == NULL) {
        fprintf(stderr, "Memory allocation for grid rows failed!\n");
        return;
    }
    for (int i = 0; i < GRID_HEIGHT; i++) {
        grid[i] = (char *)malloc(GRID_WIDTH * sizeof(char));
        if (grid[i] == NULL) {
            fprintf(stderr, "Memory allocation for grid columns failed!\n");
            // Free previously allocated rows before exiting
            for (int k = 0; k < i; k++) {
                free(grid[k]);
            }
            free(grid);
            return;
        }
    }

    // Initialize grid with spaces
    for (int i = 0; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            grid[i][j] = ' ';
        }
    }

    // Mark the center (0,0)
    int center_x = GRID_WIDTH / 2;
    int center_y = GRID_HEIGHT / 2;
    grid[center_y][center_x] = '+'; // Mark center with a plus sign

    // Draw the main circle (boundary)
    // Iterate through angles to approximate the circle
    for (double angle = 0; angle < 2 * M_PI; angle += 0.05) { // Smaller step for smoother circle
        double x_circ = max_radius * cos(angle);
        double y_circ = max_radius * sin(angle);

        // Map world coordinates to grid coordinates
        // Scale x to GRID_WIDTH, y to GRID_HEIGHT
        // Note: y-axis is inverted for console (row 0 is top)
        int plot_x = (int)((x_circ / max_radius) * (GRID_WIDTH / 2 - 1) + center_x);
        int plot_y = (int)((-y_circ / max_radius) * (GRID_HEIGHT / 2 - 1) + center_y);

        // Ensure coordinates are within bounds
        if (plot_x >= 0 && plot_x < GRID_WIDTH && plot_y >= 0 && plot_y < GRID_HEIGHT) {
            if (grid[plot_y][plot_x] == ' ') { // Only draw if not already occupied
                grid[plot_y][plot_x] = '.';
            }
        }
    }

    // Plot individual items
    for (int i = 0; i < n; i++) {
        double r = items[i].location.r;
        double theta = items[i].location.theta;

        // Convert polar to Cartesian
        double x = r * cos(theta);
        double y = r * sin(theta);

        // Map world coordinates to grid coordinates
        int plot_x = (int)((x / max_radius) * (GRID_WIDTH / 2 - 1) + center_x);
        int plot_y = (int)((-y / max_radius) * (GRID_HEIGHT / 2 - 1) + center_y);

        // Ensure coordinates are within bounds
        if (plot_x >= 0 && plot_x < GRID_WIDTH && plot_y >= 0 && plot_y < GRID_HEIGHT) {
            // Prioritize CG if there's an overlap, otherwise mark item with its number
            // Mark items 1-9 with their number, others with '#'
            if (grid[plot_y][plot_x] != 'X') { // Check for 'X' as it's the new CG marker
                if (i < 9) { // For items 1 to 9
                    grid[plot_y][plot_x] = (char)('1' + i);
                } else { // For 10th item and beyond
                    grid[plot_y][plot_x] = '#';
                }
            }
        }
    }

    // Plot Center of Gravity (CG)
    double cog_x = cog.r * cos(cog.theta);
    double cog_y = cog.r * sin(cog.theta);

    int plot_cog_x = (int)((cog_x / max_radius) * (GRID_WIDTH / 2 - 1) + center_x);
    int plot_cog_y = (int)((-cog_y / max_radius) * (GRID_HEIGHT / 2 - 1) + center_y);

    // Ensure coordinates are within bounds
    if (plot_cog_x >= 0 && plot_cog_x < GRID_WIDTH && plot_cog_y >= 0 && plot_cog_y < GRID_HEIGHT) {
        grid[plot_cog_y][plot_cog_x] = 'X'; // Mark CG with 'X'
    }

    // Print the grid
    printf("\n--- Polar Plot ---\n");
    for (int i = 0; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            printf("%c", grid[i][j]);
        }
        printf("\n");
    }
    printf("------------------\n");
    printf("Legend: '+' = Center (0,0), '.' = Circle Boundary, '1-9' = Item Number, '#' = Item (10+), 'X' = Center of Gravity\n");


    // Free the dynamically allocated grid memory
    for (int i = 0; i < GRID_HEIGHT; i++) {
        free(grid[i]);
    }
    free(grid);
}

int inputItems(Item *items,int num_items){
    // Get input for each item
    for (int i = 0; i < num_items; i++) {
        printf("\n--- Item %d ---\n", i + 1);
        printf("Enter weight: ");
        if (scanf("%lf", &items[i].weight) != 1) {
            fprintf(stderr, "Invalid input for weight. Exiting.\n");
            free(items);
            return 1;
        }

        printf("Enter radius (r): ");
        if (scanf("%lf", &items[i].location.r) != 1) {
            fprintf(stderr, "Invalid input for radius. Exiting.\n");
            free(items);
            return 1;
        }
        // Update max_r_for_plot if current item's radius is larger
        if (items[i].location.r > max_r_for_plot) {
            max_r_for_plot = items[i].location.r;
        }

        printf("Enter angle in degrees (theta): ");
        double angle_degrees;
        if (scanf("%lf", &angle_degrees) != 1) {
            fprintf(stderr, "Invalid input for angle. Exiting.\n");
            free(items);
            return 1;
        }
        items[i].location.theta = angle_degrees * (M_PI / 180.0); // Convert degrees to radians
    }
}