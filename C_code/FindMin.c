#include <stdio.h>

// Function returns pointer to static array: [0] = min, [1] = 1 if unique, 0 if not
double* findMinimum(const double arr[], int size) {
    static double result[2];  // [0] = min, [1] = uniqueness flag

    if (size <= 0) {
        printf("Array is empty.\n");
        result[0] = 0.0;
        result[1] = 0.0;
        return result;
    }

    double min = arr[0];
    int count = 1;

    for (int i = 1; i < size; i++) {
        if (arr[i] < min) {
            min = arr[i];
            count = 1;
        } else if (arr[i] == min) {
            count++;
        }
    }

    result[0] = min;
    result[1] = (count == 1) ? 1.0 : 0.0;

    return result;
}

int main() {
    double array[] = {5.2, 3.1, 4.8, 3.1, 6.0, 7.2};
    int size = sizeof(array) / sizeof(array[0]);

    double *result = findMinimum(array, size);

    printf("Minimum value: %.6f\n", result[0]);
    if (result[1] == 0.0) {
        printf("Note: The minimum value is not unique.\n");
    } else {
        printf("The minimum value is unique.\n");
    }

    return 0;
}