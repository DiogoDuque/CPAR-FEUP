#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <omp.h>
#include <mpi.h>

#define OMP_N_THREADS 4
#define OMP_CHUNK_SIZE 40
#define OMP_BIG_CHUNK_SIZE 25000000

struct Primes
{
    int *numbers;
    int size;
};

void displayNumbers(int *numbers, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d, ", numbers[i]);
    }
    printf("\n");
}

struct Primes eratosthenes(int nMax)
{
    const int nMin = 2;

    int *numbers = (int *)malloc((nMax - 1) * sizeof(int));
    for (int n = nMin; n <= nMax; n++) //init array, but only with odd numbers (except for '2')
    {
        numbers[n - nMin] = (n % 2 == 0 && n != 2) ? 0 : n;
    }

    int numberOfPrimes = 1;
    for (int i = 1; i < nMax - 1; i += 2) // starts searching on 3, as this allows as to modify the algorithm to skip even numbers
    {
        int n = numbers[i];
        if (n == 0) // if was already seen, is not a prime
            continue;

        numberOfPrimes++;
        for (int j = i + 2 * n; j < nMax - 1; j += 2 * n)
        {
            numbers[j] = 0; // mark as seen and as not prime
        }
    }

    int *primes = (int *)malloc(numberOfPrimes * sizeof(int));
    primes[0] = 2;
    numberOfPrimes = 1;
    for (int i = 1; i < nMax - 1; i += 2)
    {
        if (numbers[i] == 0)
            continue;
        primes[numberOfPrimes++] = numbers[i];
    }
    struct Primes p;
    p.numbers = primes;
    p.size = numberOfPrimes;
    return p;
}

struct Primes eratosthenesShared(int nMax)
{
    const int nMin = 2;

    int *numbers = (int *)malloc((nMax - 1) * sizeof(int));
#pragma omp parallel for num_threads(OMP_N_THREADS) schedule(static, OMP_BIG_CHUNK_SIZE)
    for (int n = nMin; n <= nMax; n++) //init array, but only with odd numbers (except for '2')
    {
        numbers[n - nMin] = (n % 2 == 0 && n != 2) ? 0 : n;
    }

    int numberOfPrimes = 1;
#pragma omp parallel for schedule(dynamic, OMP_CHUNK_SIZE) num_threads(OMP_N_THREADS) reduction(+:numberOfPrimes)
    for (int i = 1; i < nMax - 1; i += 2) // starts searching on 3, as this allows as to modify the algorithm to skip even numbers
    {
        int n = numbers[i];
        if (n == 0) // if was already seen, is not a prime
            continue;

        numberOfPrimes += 1; // found a prime
        for (int j = i + 2 * n; j < nMax - 1; j += 2 * n)
        {
            numbers[j] = 0; // mark as seen and as not prime
        }
    }

    int *primes = (int *)malloc(numberOfPrimes * sizeof(int));
    primes[0] = 2;
    numberOfPrimes = 1;
#pragma omp parallel for num_threads(OMP_N_THREADS) schedule(static, OMP_BIG_CHUNK_SIZE)
    for (int i = 1; i < nMax - 1; i += 2)
    {
        if (numbers[i] == 0)
            continue;
#pragma omp critical //cannot make a reduction because numberOfPrimes is needed after the increment
        primes[numberOfPrimes++] = numbers[i];
    }
    struct Primes p;
    p.numbers = primes;
    p.size = numberOfPrimes;
    return p;
}

int main(int argc, char **argv)
{
    const char usageStr[] = "Usage: eratosthenes <seq|shared>\nseq: Sequential Program\nshared: Parallel Program with Shared memory\n";
    if (argc != 2)
    {
        printf(usageStr);
        return 1;
    }
    bool isSequential = false, isParShared = false;
    if (strcmp("seq", argv[1]) == 0)
        isSequential = true;
    else if (strcmp("shared", argv[1]) == 0)
        isParShared = true;
    else if (strcmp("test", argv[1]) != 0)
    {
        printf(usageStr);
        return 1;
    }

    int MAX_N = 600000000;
    for (int i = 100000000; i <= MAX_N; i += 100000000)
    {
        struct timespec t_start, t_stop;
        clock_gettime(CLOCK_MONOTONIC, &t_start);

        struct Primes p;
        if (isSequential)
        {   
            printf("======= Running Sequential...\n");
            p = eratosthenes(i);
        }
        else if (isParShared)
        {
            printf("=======Running Shared... Threads: %d, ChunkSize: %d\n",OMP_N_THREADS, OMP_CHUNK_SIZE);
            p = eratosthenesShared(i);
        }
        else
        {
            printf("=======Running Test...\n");
            p = eratosthenesShared(120);
        }

        clock_gettime(CLOCK_MONOTONIC, &t_stop);
        float deltaTime = (float) (( t_stop.tv_sec - t_start.tv_sec ) + ( t_stop.tv_nsec - t_start.tv_nsec ) / 1000000000.0); //in seconds

        printf("Found %d primes in %.4lfs\n", p.size, deltaTime);
        //displayNumbers(p.numbers, p.size);
    }

    return 0;
}