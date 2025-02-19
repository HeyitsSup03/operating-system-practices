#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int data[20];
int sortedData[20];

struct SortParams {
    int start_index;
    int end_index;
};

void merge(int start, int mid, int end) {
    int mergedSize = end - start + 1;
    int tempArray[mergedSize];
    int leftPos = start;
    int rightPos = mid + 1;
    int mergePos = 0;

    while (leftPos <= mid && rightPos <= end) {
        if (data[leftPos] < data[rightPos]) {
            tempArray[mergePos++] = data[leftPos++];
        } else {
            tempArray[mergePos++] = data[rightPos++];
        }
    }

    while (leftPos <= mid) {
        tempArray[mergePos++] = data[leftPos++];
    }

    while (rightPos <= end) {
        tempArray[mergePos++] = data[rightPos++];
    }

    for (mergePos = 0; mergePos < mergedSize; ++mergePos) {
        data[start + mergePos] = tempArray[mergePos];
    }
}

void mergeSort(int start, int end) {
    if (start < end) {
        int mid = (start + end) / 2;
        mergeSort(start, mid);
        mergeSort(mid + 1, end);
        merge(start, mid, end);
    }
}

void *sortingThread(void *param) {
    struct SortParams *params = (struct SortParams *)param;
    mergeSort(params->start_index, params->end_index);
    pthread_exit(NULL);
}

void *mergingThread(void *param) {
    int n = *((int *)param);
    merge(0, (n - 1) / 2, n - 1);
    pthread_exit(NULL);
}

int main() {
    int n;
    printf("Enter number of integers for the array: ");
    scanf("%d",&n);
    printf("Enter %d integers for the array:\n", n);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &data[i]);
    }
    pthread_t sortThread1, sortThread2, mergeThread;
    struct SortParams params1 = {0, (n - 1) / 2};
    struct SortParams params2 = {(n - 1) / 2 + 1, n - 1};
    pthread_create(&sortThread1, NULL, sortingThread, &params1);
    pthread_create(&sortThread2, NULL, sortingThread, &params2);
    pthread_join(sortThread1, NULL);
    pthread_join(sortThread2, NULL);
    pthread_create(&mergeThread, NULL, mergingThread, (void *)&n);
    pthread_join(mergeThread, NULL);
    printf("Sorted array: ");
    for (int i = 0; i < n; ++i) {
        printf("%d ", data[i]);
    }
    printf("\n");

    return 0;
}
