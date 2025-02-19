#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

#define READ_END 0
#define WRITE_END 1

int main() {
    srand(time(NULL));
    int pipe1[2], pipe2[2];
    if(pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }
    pid_t child1, child2;
    child1 = fork();
    if(child1 == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    if(child1 == 0) {
        close(pipe1[READ_END]);
        close(pipe2[WRITE_END]);
        srand(getpid());
        while(1) {
            int num = rand() % 100;
            write(pipe1[WRITE_END], &num, sizeof(int));
            usleep(100000);
        }
    }
    child2 = fork();
    if(child2 == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    if(child2 == 0) {
        close(pipe1[WRITE_END]);
        close(pipe2[READ_END]);
        srand(getpid());
        while(1) {
            int num = rand() % 100;
            write(pipe2[WRITE_END], &num, sizeof(int));
            usleep(100000);
        }
    }
    close(pipe1[WRITE_END]);
    close(pipe2[WRITE_END]);
    int MAX_SCORE;
    printf("Enter maximum score: ");
    scanf("%d", &MAX_SCORE);
    int score_c = 0, score_d = 0, winner = 0, round = 0;
    while(1) {
        round++;
        printf("Round %d:\n", round);
        int flag = rand() % 2;
        printf("P's choice: %s\n", (flag == 0) ? "MIN" : "MAX");
        int c, d;
        read(pipe1[READ_END], &c, sizeof(int));
        read(pipe2[READ_END], &d, sizeof(int));
        printf("Integers received from C and D: %d %d\n", c, d);
        if(c == d) {
            printf("Round ignored, integers are equal.\n");
        } else {
            if((flag == 0 && c < d) || (flag == 1 && c > d)) {
                score_c++;
                printf("C gets 1 point.\n");
            } else {
                score_d++;
                printf("D gets 1 point.\n");
            }
            printf("Current scores: C - %d, D - %d\n", score_c, score_d);
            if(score_c == MAX_SCORE || score_d == MAX_SCORE) {
                winner = (score_c == MAX_SCORE) ? 1 : 2;
                break;
            }
        }
        usleep(500000);
    }
    kill(child1, SIGUSR1);
    kill(child2, SIGUSR1);
    wait(NULL);
    wait(NULL);
    printf("Game over. Winner: %s\n", (winner == 1) ? "C" : "D");
    return 0;
}

