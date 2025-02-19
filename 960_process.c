#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void task1(int arr[], int n) {
    int min = arr[0];
    for (int i = 1; i < n; ++i) {
        if (arr[i] < min) {
            min = arr[i];
        }
    }
    printf("Child process (PID %d, PPID %d) - Minimum value: %d\n", getpid(), getppid(), min);
}

void task2(int arr[], int n) {
    int max = arr[0];
    for (int i = 1; i < n; ++i) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    printf("Parent process (PID %d) - Maximum value: %d\n", getpid(), max);
}

int main() {
    int n;

    printf("Enter the number of integers: ");
    scanf("%d", &n);

    int arr[n];

    printf("Enter %d integers:\n", n);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &arr[i]);
    }

    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        // Child process
        task1(arr, n);
    } else {
        // Parent process
        // Wait for the child process to complete task1
        sleep(2);
        wait(NULL);

        // Continue with task2
        task2(arr, n);
    }

    return 0;
}

