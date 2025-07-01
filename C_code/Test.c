#include <stdio.h>
#include <stdlib.h>

#define NUM_ARRAYS 4
#define ARRAY_SIZE 5

// Compare two arrays lexicographically
int compareArrays(double a[], double b[], int size) {
    for (int i = 0; i < size; i++) {
        if (a[i] < b[i]) return -1;
        if (a[i] > b[i]) return 1;
    }
    return 0; // Arrays are identical
}



int main() {
    // Example arrays
    double arrays[NUM_ARRAYS][ARRAY_SIZE] = {
        {3.0, 2.5, 4.1, 6.0, 7.2},
        {1.1, 2.0, 3.3, 4.4, 5.5},
        {1.1, 2.0, 3.5, 4.0, 5.0},
        {5.5, 6.0, 7.0, 8.0, 9.0}
    };

    double sorted_arrays[NUM_ARRAYS][ARRAY_SIZE];
    double min_values[NUM_ARRAYS];

    // Sort each array and extract its min value
    for (int i = 0; i < NUM_ARRAYS; i++) {
        sortArray(arrays[i], sorted_arrays[i], ARRAY_SIZE);
        min_values[i] = sorted_arrays[i][0];
    }

    // Find global minimum
    double global_min = min_values[0];
    for (int i = 1; i < NUM_ARRAYS; i++) {
        if (min_values[i] < global_min)
            global_min = min_values[i];
    }

    // Identify arrays with that global min
    int candidates[NUM_ARRAYS];
    int candidate_count = 0;
    for (int i = 0; i < NUM_ARRAYS; i++) {
        if (min_values[i] == global_min) {
            candidates[candidate_count++] = i;
        }
    }

    // If only one array has the min, return it
    if (candidate_count == 1) {
        printf("Chosen array index: %d\n", candidates[0]);
        return 0;
    }

    // Tie-breaker: Compare full sorted arrays lexicographically
    int best_index = candidates[0];
    for (int i = 1; i < candidate_count; i++) {
        int cmp = compareArrays(sorted_arrays[candidates[i]], sorted_arrays[best_index], ARRAY_SIZE);
        if (cmp < 0) {
            best_index = candidates[i];
        }
    }

    printf("Chosen array index after tie-break: %d\n", best_index);
    return 0;
}