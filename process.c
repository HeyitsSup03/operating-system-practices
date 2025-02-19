#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


#define MIN_VALUE 50
#define MAX_VALUE 100

int generateRandomInt() {
    return rand() % (MAX_VALUE - MIN_VALUE + 1) + MIN_VALUE;
}

int computeMax(int *array, int l, int r) {
    int max = array[l];
    for (int i = l + 1; i <= r; i++) {
        if (array[i] > max) {
            max = array[i];
        }
    }
    return max;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <array_size>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        printf("Array size must be a positive integer.\n");
        return 1;
    }

    // Seed for random number generation
    srand(time(NULL));

    // Allocate and populate the array with random integers
    int *array = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        array[i] = generateRandomInt();
    }

    // Print the initially unsorted array
    printf("Initially unsorted array:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    // Process creation and maximum computation in the main function
    int result;
    pid_t leftChild, rightChild;
    int m = n / 2;

    if ((rightChild = fork()) == 0) {
        // Child process (right)
        result = computeMax(array, m, n - 1);
        printf("PID: %d, PPID: %d, Maximum: %d\n", getpid(), getppid(), result);
        exit(result);
    }

    if ((leftChild = fork()) == 0) {
        // Child process (left)
        result = computeMax(array, 0, m - 1);
        printf("PID: %d, PPID: %d, Maximum: %d\n", getpid(), getppid(), result);
        exit(result);
    }

    // Parent process
    int status;
    waitpid(leftChild, &status, 0);
    int leftMax = WEXITSTATUS(status);

    waitpid(rightChild, &status, 0);
    int rightMax = WEXITSTATUS(status);

    // Compute and print the maximum of the two child processes
    result = (leftMax > rightMax) ? leftMax : rightMax;
    printf("PID: %d, PPID: %d, Maximum: %d\n", getpid(), getppid(), result);

    // Print the overall maximum computed by the root process
    printf("Overall Maximum: %d\n", result);

    free(array);

    return 0;
}

