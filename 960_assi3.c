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

        // Sleep to become a zombie process
        sleep(5);

        // Child process terminates
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

        // Print process status using ps command
        printf("Parent Process Status:\n");
        fflush(stdout);
        system("ps -p $$ -o pid,ppid,state,cmd");

        // Parent process goes to sleep, leaving the child as a zombie
        sleep(10);

        // Kill the parent process using the kill command (replace <parent_pid> with actual parent process ID)
        // This will allow the child process to terminate
        // You can find the parent process ID using the ps command before running the program
        // For example: ps aux | grep process
        // Then use: kill -9 <parent_pid>
        // Note: Be cautious when using kill -9 as it forcefully terminates the process
        // The parent process will be terminated, and the child will be re-parented to init process

        // Uncomment the following line and replace <parent_pid> with the actual parent process ID
        // kill -9 <parent_pid>;

        exit(EXIT_SUCCESS);
    }

    return 0;
}
