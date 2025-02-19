#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    // Number of integers
    int n = 5;

    // Array to store integers
    int arr[] = {10, 5, 8, 15, 3};

    // Create a child process
    pid_t child_pid = fork();

    if (child_pid == -1) {
        // Fork failed
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (child_pid == 0) {
        // Child process
        printf("Child Process ID: %d\n", getpid());
        printf("Child Parent Process ID: %d\n", getppid());

        // Task 1: Print the minimum of n integers
        int min = arr[0];
        for (int i = 1; i < n; i++) {
            if (arr[i] < min) {
                min = arr[i];
            }
        }
        printf("Child Process - Minimum of %d integers: %d\n", n, min);

        // Sleep to ensure completion of Task 1 before Task 2
        sleep(2);

        exit(EXIT_SUCCESS);
    } else {
        // Parent process
        printf("Parent Process ID: %d\n", getpid());
        printf("Parent Parent Process ID: %d\n", getppid());

        // Task 2: Print the maximum of n integers
        int max = arr[0];
        for (int i = 1; i < n; i++) {
            if (arr[i] > max) {
                max = arr[i];
            }
        }
        printf("Parent Process - Maximum of %d integers: %d\n", n, max);

        // Wait for the child to complete
        wait(NULL);

        exit(EXIT_SUCCESS);
    }

    return 0;
