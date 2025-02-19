#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define MAX_SIZE 10

typedef struct {
    int buffer[MAX_SIZE];
    int head;
    int tail;
    int count;
    int turn;
    int flag[2];
} queue;

void producer(queue *q) {
    int i;
    for (i = 0; i < 10; i++) {
        while (q->count == MAX_SIZE) {
            q->flag[0] = 1;
            while (q->flag[0] && q->turn == 1) {
                q->flag[0] = 0;
                printf("Producer is waiting\n");
            }
        }
        q->buffer[q->tail] = i;
        q->tail = (q->tail + 1) % MAX_SIZE;
        q->count++;
        printf("Produced item %d\n", i);
        q->flag[1] = 1;
        while (q->flag[1] && q->turn == 0) {
            q->flag[1] = 0;
            printf("Producer is waiting\n");
        }
    }
}

void consumer(queue *q) {
    int i;
    for (i = 0; i < 10; i++) {
        while (q->count == 0) {
            q->flag[1] = 1;
            while (q->flag[1] && q->turn == 1) {
                q->flag[1] = 0;
                printf("Consumer is waiting\n");
            }
        }
        printf("Consumed item %d\n", q->buffer[q->head]);
        q->head = (q->head + 1) % MAX_SIZE;
        q->count--;
        q->flag[0] = 1;
        while (q->flag[0] && q->turn == 0) {
            q->flag[0] = 0;
            printf("Consumer is waiting\n");
        }
    }
}

int main() {
    int shmid;
    queue *q;
    pid_t pid;

    shmid = shmget(IPC_PRIVATE, sizeof(queue), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    q = (queue *)shmat(shmid, NULL, 0);
    if (q == (void *)-1) {
        perror("shmat");
        exit(1);
    }

    q->head = 0;
    q->tail = 0;
    q->count = 0;
    q->turn = 0;
    q->flag[0] = 0;
    q->flag[1] = 0;

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        producer(q);
    } else {
        consumer(q);
    }

    shmdt(q);
    shmctl(shmid, IPC_RMID, 0);

    return 0;
}