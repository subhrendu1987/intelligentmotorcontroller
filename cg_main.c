#include<stdio.h>
#include "CG.h"

int main() {
    int num_items = 0; // Pointer for number of items
    Item *items = NULL;      // Pointer for array of items
    PolarCoord cog_result;   // To store the calculated center of gravity
    double max_r_for_plot = 0.0; // To determine the scale of the plot
    char confirm_char;       // To store user's confirmation input


    // Prompt for number of items
    printf("Enter the number of items: ");
    if (scanf("%d", &num_items) != 1 || num_items <= 0) {
        fprintf(stderr, "Invalid input. Please enter a positive integer for number of items.\n");
        return 1;
    }

    // Dynamically allocate memory for the array of items
    items = (Item *)malloc(num_items * sizeof(Item));
    if (items == NULL) {
        fprintf(stderr, "Memory allocation for items failed! Exiting.\n");
        return 1;
    }

    if(num_items!=inputItems(items,num_items, &max_r_for_plot)){
        fprintf(stderr, "Memory allocation for items failed! Exiting.\n");
        return 1;
    }

    // Calculate the center of gravity
    cog_result = calculateCenterOfGravityPolar(items, num_items);

    // Update max_r_for_plot if COG radius is larger
    if (cog_result.r > max_r_for_plot) {
        max_r_for_plot = cog_result.r;
    }
    // Ensure a minimum radius for plotting, especially if all points are at origin
    if (max_r_for_plot == 0.0) {
        max_r_for_plot = 1.0;
    }
    // Get confirmation from user before plotting
    printf("\nDo you want to plot the results? (y/n): ");
    // Clear the input buffer before reading the character
    while (getchar() != '\n');
    scanf("%c", &confirm_char);

    if (confirm_char == 'y' || confirm_char == 'Y') {
        // Draw the plot only if user confirms
        drawPolarPlot(items, num_items, cog_result, max_r_for_plot);
    } else {
        printf("Plotting skipped as per user's request.\n");
    }

    // Display the calculated center of gravity
    printf("\nCalculated Center of Gravity (COG):\n");
    printf("  Radius (R): %.2f\n", cog_result.r);
    printf("  Angle (Theta): %.2f radians (%.2f degrees)\n",
           cog_result.theta, cog_result.theta * (180.0 / M_PI));

    // Free the dynamically allocated items array
    free(items);
    items = NULL;

    return 0; // Indicate successful execution
}
