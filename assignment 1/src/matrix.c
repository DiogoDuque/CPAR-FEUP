#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <papi.h>
#include <omp.h>
#include <stdbool.h>

struct Event
{
    int id;
    char name[50];
};

const struct Event events[] = {
    //{PAPI_SP_OPS, "FLOP"}, //if this is uncommented, DMCs will not be measured
    {PAPI_L1_DCM, "DCM(L1)"},
    {PAPI_L2_DCM, "DCM(L2)"},
};

int createEventSet()
{
    int ret;
    int EventSet = PAPI_NULL;

    ret = PAPI_library_init(PAPI_VER_CURRENT);
    if (ret != PAPI_VER_CURRENT)
        printf("ERROR: Failed version\n");

    ret = PAPI_create_eventset(&EventSet);
    if (ret != PAPI_OK)
        printf("ERROR on create eventset\n");

    for (int i = 0; i < sizeof(events) / sizeof(events[0]); i++)
    {
        ret = PAPI_add_event(EventSet, events[i].id);
        if (ret != PAPI_OK)
            printf("ERROR %d on adding event %s\n", ret, events[i].name);
    }
    printf("\n");

    return EventSet;
}

float *generateZeroMatrix(int n)
{
    float *m = malloc(n * n * sizeof(float));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            *(m + i * n + j) = 0;
        }
    }
    return m;
}

float *generateMatrix(int n)
{
    float *m = malloc(n * n * sizeof(float));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            *(m + i * n + j) = rand() % 10;
        }
    }

    return m;
}

float *multiplyMatrices(float *m1, float *m2, int n)
{
    float *m = generateZeroMatrix(n);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            //printf("(%d, %d)\n",i,j);
            for (int t = 0; t < n; t++)
            {
                *(m + i * n + j) += *(m1 + i * n + t) * *(m2 + t * n + j);
            }
        }
    }

    return m;
}

float *multiplyMatricesP(float *m1, float *m2, int n, int nThreads)
{
    float *m = generateZeroMatrix(n);

#pragma omp parallel for num_threads(nThreads)
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            for (int t = 0; t < n; t++)
            {
                *(m + i * n + j) += *(m1 + i * n + t) * *(m2 + t * n + j);
            }
        }
    }

    return m;
}

float *multiplyMatricesV2(float *m1, float *m2, int n)
{
    float *m = generateZeroMatrix(n);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            float m1Element = *(m1 + i * n + j);
            for (int t = 0; t < n; t++)
            {
                *(m + i * n + t) += m1Element * *(m2 + j * n + t);
            }
        }
    }

    return m;
}

float *multiplyMatricesV2P(float *m1, float *m2, int n, int nThreads)
{
    float *m = generateZeroMatrix(n);

#pragma omp parallel for num_threads(nThreads)
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            float m1Element = *(m1 + i * n + j);
            for (int t = 0; t < n; t++)
            {
                *(m + i * n + t) += m1Element * *(m2 + j * n + t);
            }
        }
    }

    return m;
}

void generateAndMultiplyMatrices(int n, char *algorithmVersion, int nThreads)
{
    float *m1 = generateMatrix(n);
    float *m2 = generateMatrix(n);
    float *m3;

    if (!strcmp("v1", algorithmVersion))
    {
        if (nThreads > 0)
        {
            //printf("v1 threaded\n");
            m3=multiplyMatricesP(m1, m2, n, nThreads);
        }
        else
        {
            //printf("v1 no threads\n");
            m3=multiplyMatrices(m1, m2, n);
        }
    }
    else
    {
        if (nThreads > 0)
        {
            //printf("v2 threaded\n");
            m3=multiplyMatricesV2P(m1, m2, n, nThreads);
        }
        else
        {
            //printf("v2 no threads\n");
            m3=multiplyMatricesV2(m1, m2, n);
        }
    }
    free(m1);
    free(m2);
    free(m3);
}

int main(int argc, char **argv)
{
    if (argc < 2 || argc > 4 ||
        (!strcmp(argv[1], "v1") && !strcmp(argv[1], "v2")) ||
        (argc == 3 && strcmp(argv[2], "parallel")))
    {
        printf("\nUsage: cMatrix < v1 | v2 > [parallel]\n");
        printf("v1 is a naive approach; v2 is uses row-major order\n");
        printf("'parallel' is an optional argument for parallel computing\n\n");
        return -1;
    }

    bool parallel;
    if (argc == 3)
        parallel = true;
    else
        parallel = false;

    //PAPI inits
    int ret;
    int EventSet = createEventSet();
    const int nEvents = sizeof(events) / sizeof(events[0]);

    //starting
    srand(time(NULL));
    if (parallel)
    {
        for (int i = 500; i <= 3000; i += 500)
        {
            for (int threads = 1; threads <= 8; threads++)
            {
                long long startTime = PAPI_get_real_usec();
                generateAndMultiplyMatrices(i, argv[1], threads);
                long long endTime = PAPI_get_real_usec();
                double deltaTime = ((double)endTime - startTime) / 1000000; //in seconds
                printf("%dx%d matrix (%s):\n", i, i, argv[1]);
                printf("Threads: %d\n",threads);
                printf("Time: %.3lfs\n\n",deltaTime);
            
            }
        }
    }
    else
    {
        for (int i = 500; i <= 3000; i += 500)
        //for(int i=5000; i<=10000; i+=1000)
        {
            long long papiVals[nEvents];
            //start counting with PAPI
            long long startTime = PAPI_get_real_usec();
            ret = PAPI_start(EventSet);
            if (ret != PAPI_OK)
                printf("ERROR: Start PAPI\n");

            //matrices calc
            generateAndMultiplyMatrices(i, argv[1], -1);

            //stop counting PAPI and print
            ret = PAPI_stop(EventSet, papiVals);
            if (ret != PAPI_OK)
                printf("ERROR: Stop PAPI\n");
            long long endTime = PAPI_get_real_usec();

            printf("%dx%d matrix (%s):\n", i, i, argv[1]);
            for (int j = 0; j < nEvents; j++)
            {
                printf("%s: %lld\n", events[j].name, papiVals[j]);
            }
            long long totalDCM = papiVals[0] + papiVals[1];
            long long flop = 2 * ((long long)pow(i, 3));
            double deltaTime = ((double)endTime - startTime) / 1000000; //in seconds
            long long flops = flop / deltaTime;                         //(time in seconds)
            float cacheMissesPerFlop = (float)totalDCM / flop;
            printf("Total DCM: %lld\nFLOP: %lld\nFLOPS: %lld\nCM/FLOP: %.3f\nTime: %.3lfs\n\n", totalDCM, flop, flops, cacheMissesPerFlop, deltaTime);

            ret = PAPI_reset(EventSet);
            if (ret != PAPI_OK)
                printf("FAIL reset\n");
        }
    }

    //remove events and destroy eventset
    for (int i = 0; i < nEvents; i++)
    {
        ret = PAPI_remove_event(EventSet, events[i].id);
        if (ret != PAPI_OK)
            printf("FAIL on remove event %s\n", events[i].name);
    }

    ret = PAPI_destroy_eventset(&EventSet);
    if (ret != PAPI_OK)
        printf("FAIL on destroy\n");

    return 0;
}
