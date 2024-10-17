#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <string.h>
#include <stddef.h>

#define MAX 1000000 
#define NUM_THREADS 4 // MAXXED AT 32

unsigned char *sieve;
int sqrt_max;

typedef struct {
    int start;
    int end;
    int prime;
} sieve_task;

void* sieve_prime(void* arg) {
    sieve_task *task = (sieve_task*) arg;
    int start = (task->start + task->prime - 1) / task->prime * task->prime;
    start = (start < task->prime * task->prime) ? task->prime * task->prime : start;
    
    for(int multiple = start; multiple <= task->end; multiple += task->prime) {
        sieve[multiple / 8] |= (1 << (multiple % 8));
    }
    return NULL;
}

int is_prime(int n) {
    return !(sieve[n / 8] & (1 << (n % 8)));
}

void mark_prime(int n) {
    sieve[n / 8] &= ~(1 << (n % 8)); // Set the bit to 0
}

int main() {
    sqrt_max = (int)sqrt(MAX);
    size_t sieve_size = (MAX + 7) / 8; // We divide by 8 to get the number of bytes
    // Dynamically allocate the sieve array and initializes all bits to zero 
    sieve = (unsigned char*)calloc(sieve_size, sizeof(unsigned char));
    
    sieve[0] = 0x03;

    pthread_t threads[NUM_THREADS];
    sieve_task *tasks = malloc(NUM_THREADS * sizeof(sieve_task));

    for(int p = 2; p <= sqrt_max; p++) {
        if(is_prime(p)) {
            int range = MAX - p * p;
            int chunk = (range + NUM_THREADS - 1) / NUM_THREADS;

            for(int i = 0; i < NUM_THREADS; ++i) {
                tasks[i].start = p * p + i * chunk;
                tasks[i].end = (i == NUM_THREADS - 1) ? MAX : tasks[i].start + chunk - 1;
                tasks[i].prime = p;
                pthread_create(&threads[i], NULL, sieve_prime, (void*)&tasks[i]);
            }

            for(int i = 0; i < NUM_THREADS; ++i) {
                pthread_join(threads[i], NULL);
            }
        }
    }

    FILE *output_file = fopen("primes_output.txt", "w");
    if (output_file == NULL) {
        perror("Error opening file");
        return 1;
    }

    int count = 0;
    for(int i = 2; i <= MAX; ++i) {
        if(is_prime(i)) {
            count++;
            fprintf(output_file, "%d ", i);  // Write to file
            if(count <= 100) printf("%d ", i);  // Print to console (truncated)
        }
    }

    fprintf(output_file, "\nTotal primes found: %d\n", count);
    // print truncated output
    printf("\n... (truncated output) check primes_output.txt for complete list\n");
    printf("\nTotal primes found: %d\n", count);

    fclose(output_file);

    free(sieve);
    free(tasks);
    return 0;
}
