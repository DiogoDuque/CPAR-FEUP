#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <omp.h>
#include <mpi.h>
#include "csvHelper.h"

#define OMP_N_THREADS 4
#define OMP_CHUNK_SIZE 40
#define OMP_BIG_CHUNK_SIZE 25000000

struct Primes
{
    long int *numbers;
    long int size;
};

void displayNumbers(long int *numbers, long int size)
{
    for (long int i = 0; i < size; i++)
    {
        printf("%ld, ", numbers[i]);
    }
    printf("\n");
}

struct Primes eratosthenes(long int nMax)
{
    const long int nMin = 2;

    long int *numbers = (long int *)malloc((nMax - 1) * sizeof(long int));
    for (long int n = nMin; n <= nMax; n++) //init array, but only with odd numbers (except for '2')
    {
        numbers[n - nMin] = (n % 2 == 0 && n != 2) ? 0 : n;
    }

    long int numberOfPrimes = 1;
    for (long int i = 1; i < nMax - 1; i += 2) // starts searching on 3, as this allows as to modify the algorithm to skip even numbers
    {
        long int n = numbers[i];
        if (n == 0) // if was already seen, is not a prime
            continue;

        numberOfPrimes++;
        for (long int j = i + 2 * n; j < nMax - 1; j += 2 * n)
        {
            numbers[j] = 0; // mark as seen and as not prime
        }
    }

    long int *primes = (long int *)malloc(numberOfPrimes * sizeof(long int));
    primes[0] = 2;
    numberOfPrimes = 1;
    for (long int i = 1; i < nMax - 1; i += 2)
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

struct Primes eratosthenesShared(long int nMax)
{
    const long int nMin = 2;

    long int *numbers = (long int *)malloc((nMax - 1) * sizeof(long int));
#pragma omp parallel for num_threads(OMP_N_THREADS) schedule(static, OMP_BIG_CHUNK_SIZE)
    for (long int n = nMin; n <= nMax; n++) //init array, but only with odd numbers (except for '2')
    {
        numbers[n - nMin] = (n % 2 == 0 && n != 2) ? 0 : n;
    }

    long int numberOfPrimes = 1;
#pragma omp parallel for schedule(dynamic, OMP_CHUNK_SIZE) num_threads(OMP_N_THREADS) reduction(+ \
                                                                                                : numberOfPrimes)
    for (long int i = 1; i < nMax - 1; i += 2) // starts searching on 3, as this allows as to modify the algorithm to skip even numbers
    {
        long int n = numbers[i];
        if (n == 0) // if was already seen, is not a prime
            continue;

        numberOfPrimes += 1; // found a prime
        for (long int j = i + 2 * n; j < nMax - 1; j += 2 * n)
        {
            numbers[j] = 0; // mark as seen and as not prime
        }
    }

    long int *primes = (long int *)malloc(numberOfPrimes * sizeof(long int));
    primes[0] = 2;
    numberOfPrimes = 1;
#pragma omp parallel for num_threads(OMP_N_THREADS) schedule(static, OMP_BIG_CHUNK_SIZE)
    for (long int i = 1; i < nMax - 1; i += 2)
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

int main(int argc, char **argv)
{
    const char usageStr[] = "Usage: eratosthenes <seq|shared> n\nseq: Sequential Program\nshared: Parallel Program with Shared memory\n";
    if (argc != 3)
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

    long int i = atol(argv[2]);

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
        printf("=======Running Shared... Threads: %d, ChunkSize: %d\n", OMP_N_THREADS, OMP_CHUNK_SIZE);
        p = eratosthenesShared(i);
    }
    else
    {
        return 1;
    }

    clock_gettime(CLOCK_MONOTONIC, &t_stop);
    float deltaTime = (float)((t_stop.tv_sec - t_start.tv_sec) + (t_stop.tv_nsec - t_start.tv_nsec) / 1000000000.0); //in seconds
    printf("Found %ld primes in %.4lfs\n", p.size, deltaTime);
    //displayNumbers(p.numbers, p.size);

    writeLineToCsv("primes.csv", p.numbers, p.size);


    return 0;
}