#include <stdio.h>
#include <pthread.h>

#define NUM_ITERS 10000
#define NUM_THREADS 100

int shared_resource = 0;
int choosing[NUM_THREADS];
int number[NUM_THREADS];

void lock(int tid) {
    choosing[tid] = 1;

    // Find the maximum ticket number
    int max = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        if (number[i] > max) {
            max = number[i];
        }
    }

    number[tid] = max + 1;
    choosing[tid] = 0;

    for (int i = 0; i < NUM_THREADS; i++) {
        if (i == tid) {
            continue;
        }

        while (choosing[i]) {
            // busy-wait
        }

        while (number[i] != 0 &&
               (number[i] < number[tid] || (number[i] == number[tid] && i < tid))) {
            // busy-wait
        }
    }
}

void unlock(int tid) {
    number[tid] = 0;
}

void* thread_func(void* arg) {
    int tid = *(int*)arg;

    lock(tid);

    for (int i = 0; i < NUM_ITERS; i++) {
        shared_resource++;
    }

    unlock(tid);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int tids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        choosing[i] = 0;
        number[i] = 0;
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        tids[i] = i;
        pthread_create(&threads[i], NULL, thread_func, &tids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("shared: %d\n", shared_resource);

    return 0;
}
