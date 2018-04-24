#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <papi.h>

struct Primes{
    int* numbers;
    int size;
};

void displayNumbers(int* numbers, int size){
    for(int i=0; i<size; i++){
        printf("%d, ",numbers[i]);
    }
    printf("\n");
}

struct Primes eratosthenes(int nMax) {
    const int nMin = 2;

    //init array
    int* numbers = (int*) malloc((nMax-1)*sizeof(int));
    for(int n=nMin; n<=nMax; n++) {
        numbers[n-nMin] = n;
    }

    int numberOfPrimes = 0;
    for(int i=0; i<nMax-1; i++){
        int n = numbers[i];
        if(n == 0) // if was already seen, is not a prime
            continue;
        numberOfPrimes++;
        for(int j=i+n; j<nMax-1; j+=n){
            numbers[j]=0; // mark as seen and as not prime
        }
    }

    int* primes = (int*) malloc(numberOfPrimes*sizeof(int));
    numberOfPrimes = 0;
    for(int i=0; i<nMax-1; i++){
        if(numbers[i] == 0)
            continue;
        primes[numberOfPrimes++]=numbers[i];
    }
    struct Primes p;
    p.numbers=primes;
    p.size=numberOfPrimes;
    return p;
}

int main(int argc, char **argv){
    int ret;

    ret = PAPI_library_init(PAPI_VER_CURRENT);
    if (ret != PAPI_VER_CURRENT)
        printf("ERROR: Failed version\n");
    


    int MAX_N  = 600000000;
    for(int i=100000000; i<=MAX_N; i+=100000000){
        long long startTime = PAPI_get_real_usec();
        struct Primes p = eratosthenes(i);
        long long endTime = PAPI_get_real_usec();
        double deltaTime = ((double)endTime - startTime) / 1000000; //in seconds

        printf("Found %d primes in %.3lfs\n",p.size, deltaTime);
        //displayNumbers(p.numbers, p.size);
    }
    
    return 0;
}