#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <mpi.h>

#define BLOCK_LOW(id,n,p) ((id)*(n)/(p))
#define BLOCK_HIGH(id,n,p) (BLOCK_LOW((id)+1,n,p)-1)
#define BLOCK_SIZE(id,n,p) (BLOCK_LOW((id)+1,n,p)-BLOCK_LOW(id,n,p))
#define BLOCK_OWNER(index,p,n) (((p)*(index)+1)-1)/(n))

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

// http://acc6.its.brooklyn.cuny.edu/~cisc7340/examples/mpisieves16.pdf
int main(int argc, char **argv){
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size( MPI_COMM_WORLD, &size);
   	MPI_Comm_rank( MPI_COMM_WORLD, &rank);
    if(argc != 2){
        return 1;
    }

    int n = atoi(argv[1]);

    double startTime = MPI_Wtime();
    
    // ========== START ALGORITHM ==========

    const int nMax = n;
    const int arraySize = nMax+1;
    int lowBound = BLOCK_LOW(rank, arraySize, size);
    //int highBound = BLOCK_HIGH(rank, arraySize, size);
    int blockSize = BLOCK_SIZE(rank, arraySize, size);

    int *numbers = (int *)malloc((blockSize) * sizeof(int));
    for (int i=0; i<blockSize; i++) //init array, but only with odd numbers (except for '2')
    {
        int globalNumber = lowBound+i;
        numbers[i] = ((globalNumber % 2 == 0 && globalNumber != 2) || globalNumber<=1) ? 0 : globalNumber;
    }

    int itIndex = 3;
    for(int prime = 3; prime*prime<=nMax;)
    {
        int startingIndex;
        if(prime*prime <= lowBound)
        {
            int remainder = lowBound % prime;
            if(remainder == 0) // easy divide
                startingIndex = 0;
            else
                startingIndex = prime - remainder;
            
        } else { // last iteration, as k is on the edge
            startingIndex = prime*prime - lowBound;
        }

        for(int i=startingIndex; i<blockSize; i+=prime){
            numbers[i]=0;
        }

        // find next k
        if(rank == 0){
            for(itIndex+=2; itIndex<blockSize; itIndex+=2){
                if(numbers[itIndex] != 0){
                    prime = numbers[itIndex];
                    break;
                }
            }

        }
        MPI_Bcast(&prime, 1, MPI_INT, 0, MPI_COMM_WORLD);
    }

    // count primes
    int localPrimeCount = 0;
    for(int i=0; i<blockSize; i++)
        if(numbers[i] != 0)
            localPrimeCount++;
    
    // get global count in root thread
    int globalPrimeCount = 0;
    int maxLocalPrimeCount = 0;
    MPI_Reduce(&localPrimeCount, &globalPrimeCount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&localPrimeCount, &maxLocalPrimeCount, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    // get primes
    int localPrimes[maxLocalPrimeCount];
    localPrimeCount=0;
    for(int i=0; i<blockSize; i++)
        if(numbers[i] != 0)
            localPrimes[localPrimeCount++]=numbers[i];

    // get primes on root thread
    int* globalPrimesRecv;
    if(rank == 0)
        globalPrimesRecv = (int*)malloc(maxLocalPrimeCount*size*sizeof(int));
    MPI_Gather(&localPrimes,localPrimeCount,MPI_INT,globalPrimesRecv,maxLocalPrimeCount,MPI_INT,0,MPI_COMM_WORLD);
    
    int* globalPrimes = (int*)malloc(arraySize*sizeof(int));
    globalPrimeCount = 0;
    for(int i=0; i<maxLocalPrimeCount*size; i++)
        if(globalPrimesRecv[i] > 0 && globalPrimesRecv[i] <= nMax)
            globalPrimes[globalPrimeCount++]=globalPrimesRecv[i];

    MPI_Barrier(MPI_COMM_WORLD);

    // ========== END ALGORITHM ==========

    double finishTime = MPI_Wtime();
    if(rank == 0)
    {
        printf("Found %d primes in %d numbers\nTime: %.4lfs\n", globalPrimeCount, n, finishTime-startTime);
        displayNumbers(globalPrimes, globalPrimeCount);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}