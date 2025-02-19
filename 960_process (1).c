#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define MAX_COMMAND_LENGTH 100
#define MAX_ARGUMENTS 10

void executeCommand(char *command, char *args[]) {
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        if (execvp(command, args) == -1) {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
    }
}

int main() {
    char command[MAX_COMMAND_LENGTH];
    char *args[MAX_ARGUMENTS];

    while (1) {
        printf("Yourname> ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0'; // Remove trailing newline

        // Tokenize the command
        char *token = strtok(command, " ");
        int i = 0;

        while (token != NULL && i < MAX_ARGUMENTS - 1) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }

        args[i] = NULL; // Set the last argument to NULL

        // Implement built-in commands
        if (strcmp(args[0], "who") == 0) {
            printf("User login: %s\n", getlogin());
        } else if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL) {
                fprintf(stderr, "cd: missing argument\n");
            } else {
                if (chdir(args[1]) != 0) {
                    perror("chdir");
                }
            }
        } else if (strcmp(args[0], "pwd") == 0) {
            char cwd[1024];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("Current directory: %s\n", cwd);
            } else {
                perror("getcwd");
            }
        } else if (strcmp(args[0], "mkdir") == 0) {
            if (args[1] == NULL) {
                fprintf(stderr, "mkdir: missing argument\n");
            } else {
                if (mkdir(args[1], 0777) != 0) {
                    perror("mkdir");
                }
            }
        } else if (strcmp(args[0], "rmdir") == 0) {
            if (args[1] == NULL) {
                fprintf(stderr, "rmdir: missing argument\n");
            } else {
                if (rmdir(args[1]) != 0) {
                    perror("rmdir");
                }
            }
        } else if (strcmp(args[0], "ls") == 0) {
            if (args[1] == NULL) {
                system("ls");
            } else {
                printf("ls: too many arguments\n");.
                
            }
        } else if (strcmp(args[0], "exit") == 0) {
            printf("Exiting the shell. Goodbye!\n");
            exit(EXIT_SUCCESS);
        } else {
            // Execute external commands
            executeCommand(args[0], args);
        }
    }

    return 0;
}


