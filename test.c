#include <stdio.h>
#include <math.h>

typedef struct {
    float iq;           // Motor Torque current
    float omega;        // Angular speed of the drum
    float *C;           // Temporal states of clothes
} state;

// Structure to represent an arm in the MAB problem
typedef struct {
    int id;
    int pulls;          // Number of times this arm has been pulled
    double totalReward; // Total reward obtained from this arm
} Arm;



// Function to calculate the UCB value for a given arm
double calculateUCB(Arm arm, int totalPulls) {
    if (arm.pulls == 0) {
        // If an arm hasn't been pulled yet, give it an infinite UCB to prioritize exploration
        return INFINITY; 
    }

    double exploitationTerm = arm.totalReward / arm.pulls; // Q-value (average reward)
    double explorationTerm = sqrt(2 * log(totalPulls) / arm.pulls); // UCB confidence bound

    return exploitationTerm + explorationTerm;
}

int main() {
    // Example Usage:

    // Define some arms
    Arm arm1 = {1, 10, 7.0};  // Arm 1: 10 pulls, 7.0 total reward
    Arm arm2 = {2, 5, 4.0};   // Arm 2: 5 pulls, 4.0 total reward
    Arm arm3 = {3, 0, 0.0};   // Arm 3: 0 pulls, 0.0 total reward (unexplored)

    // Total number of pulls across all arms so far
    int totalPulls = arm1.pulls + arm2.pulls + arm3.pulls;

    printf("Total pulls across all arms: %d\n", totalPulls);

    // Calculate UCB for each arm
    double ucb1 = calculateUCB(arm1, totalPulls);
    double ucb2 = calculateUCB(arm2, totalPulls);
    double ucb3 = calculateUCB(arm3, totalPulls);

    printf("UCB for Arm 1 (Pulls: %d, Reward: %.2f): %.4f\n", arm1.pulls, arm1.totalReward, ucb1);
    printf("UCB for Arm 2 (Pulls: %d, Reward: %.2f): %.4f\n", arm2.pulls, arm2.totalReward, ucb2);
    printf("UCB for Arm 3 (Pulls: %d, Reward: %.2f): %.4f\n", arm3.pulls, arm3.totalReward, ucb3);

    // In a real MAB scenario, you would select the arm with the highest UCB
    // and then update its pulls and totalReward based on the observed outcome.
    
    return 0;
}
