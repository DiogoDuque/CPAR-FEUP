#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <omp.h>

double **multiplyMatrices(double **m1, double **m2, int n)
{
    //generate zero matrix
    double **m = (double **)malloc(n * sizeof(double *));

    for (int i = 0; i < n; i++)
    {
        m[i] = (double *)malloc(n * sizeof(double));
        for (int j = 0; j < n; j++)
        {
            m[i][j] = 0;
        }
    }

    // mult matrices
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            //printf("(%d, %d)\n",i,j);
            for (int t = 0; t < n; t++)
            {
                m[i][j] += m1[i][t] * m2[t][j];
            }
        }
    }

    return m;
}

bool testMatrixEquiality(double **m1, double **m2, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            double a = m1[i][j], b = m2[i][j];
            double diff = fabs(a) - fabs(b);
            if (diff > 0.001 || diff < -0.001)
            {
                printf("ERROR on [%d][%d]: %lf!=%lf\n", i, j, m1[i][j], m2[i][j]);
                return false;
            }
        }
    }
    return true;
}

void displayTabbedMatrix(double **m, int n, int spaces)
{
    for (int i = 0; i < n; i++)
    {
        for (int t = 0; t < spaces; t++)
        {
            printf(" ");
        }
        for (int j = 0; j < n; j++)
        {
            printf("%.0f ", m[i][j]);
        }
        printf("\n");
    }
}

void displayMatrix(double **m, int n)
{
    displayTabbedMatrix(m, n, 0);
}

double **generateMatrix(int n)
{
    double **m = malloc(n * sizeof(double *));

    for (int i = 0; i < n; i++)
    {
        double *row = malloc(n * sizeof(double));
        m[i] = row;
        for (int j = 0; j < n; j++)
        {
            m[i][j] = rand() % 10;
        }
    }
    return m;
}

double **generateUpperMatrix(int n)
{
    double **m = malloc(n * sizeof(double *));

    for (int i = 0; i < n; i++)
    {
        double *row = malloc(n * sizeof(double));
        m[i] = row;
        for (int j = 0; j < n; j++)
        {
            if (i > j)
                m[i][j] = 0;
            //else m[i][j]=-1;
        }
    }
    return m;
}

double **generateLowerMatrix(int n)
{
    double **m = malloc(n * sizeof(double *));

    for (int i = 0; i < n; i++)
    {
        double *row = malloc(n * sizeof(double));
        m[i] = row;
        for (int j = 0; j < n; j++)
        {
            if (i < j)
                m[i][j] = 0;
            else if (i == j)
                m[i][j] = 1;
            //else m[i][j]=-1;
        }
    }
    return m;
}

// http://mathonline.wikidot.com/the-algorithm-for-doolittle-s-method-for-lu-decompositions
void doolittle(double **a, double **l, double **u, int n)
{
    for (int k = 0; k < n; k++)
    {
        for (int m = k; m < n; m++)  //kth row of u
        {
            u[k][m] = a[k][m];
            for (int j = 0; j < k; j++)
            {
                u[k][m] -= l[k][j] * u[j][m];
            }
        }
        for (int i = k; i < n; i++) //kth column of l
        {
            l[i][k] = a[i][k];
            for (int j = 0; j < k; j++)
            {
                l[i][k] -= l[i][j] * u[j][k];
            }
            l[i][k] /= u[k][k];
        }
    }
}

void doolittleShared(double **a, double **l, double **u, int n)
{
    for (int k = 0; k < n; k++)
    {
        for (int m = k; m < n; m++)
        { //kth row of u
            double tmp = a[k][m];
            #pragma omp parallel for reduction(-:tmp)
            for (int j = 0; j < k; j++)
            {
                tmp -= l[k][j] * u[j][m];
            }
            u[k][m] = tmp;
        }
        for (int i = k; i < n; i++)
        { //kth column of l
            double tmp = a[i][k];
            #pragma omp parallel for reduction(-:tmp)
            for (int j = 0; j < k; j++)
            {
                tmp -= l[i][j] * u[j][k];
            }
            tmp /= u[k][k];
            l[i][k] = tmp;
        }
    }
}

int main(int argc, char **argv)
{
    const char usageStr[] = "Usage: eratosthenes <seq|shared|test>\nseq: Sequential Program\nshared: Parallel Program with Shared memory\n";
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

    srand(time(NULL));

    for (int i = 1000; i <= 6000; i += 1000)
    {
        struct timespec t_start, t_stop;

        double **a = generateMatrix(i);
        double **l = generateLowerMatrix(i);
        //displayMatrix(l,i);
        double **u = generateUpperMatrix(i);

        if (isSequential)
        {
            printf("======= Running Sequential...\n");
            clock_gettime(CLOCK_MONOTONIC, &t_start);
            doolittle(a, l, u, i);
        }
        else if (isParShared)
        {
            printf("=======Running Shared...\n");
            clock_gettime(CLOCK_MONOTONIC, &t_start);
            doolittleShared(a, l, u, i);
        }
        else
        {
            printf("=======Running Test...\n");
            clock_gettime(CLOCK_MONOTONIC, &t_start);
            i = 10;
            doolittleShared(a, l, u, i);
        }

        clock_gettime(CLOCK_MONOTONIC, &t_stop);
        double deltaTime = (t_stop.tv_sec - t_start.tv_sec) + (t_stop.tv_nsec - t_start.tv_nsec) / 1000000000.0; //in seconds
        printf("Calculated %dx%d in %.4lfs\n", i, i, deltaTime);
        testMatrixEquiality(a, multiplyMatrices(l, u, i), i);

        /*printf("\n");
        displayMatrix(multiplyMatrices(l,u,i),i);//*/
    }

    return 0;
}