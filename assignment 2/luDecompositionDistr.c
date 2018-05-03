#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "csvHelper.h"

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
        }
    }
    return m;
}

int main(int argc, char **argv)
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (argc != 3)
    {
        printf("Usage: lu filename n\n");
        return 1;
    }

    char *filename = argv[1];
    int n = atoi(argv[2]);
    double **a = getMatrixFromCsv(filename, n);
    if (a == NULL)
    {
        printf("Error: invalid csv\n");
        return 1;
    }
    double **l = generateLowerMatrix(n);
    double **u = generateUpperMatrix(n);

    double startTime = MPI_Wtime();

    // ========== START ALGORITHM ==========

    for (int k = 0; k < n; k++)
    {
        const int offset = (rank - k + size) % size;
        const int rankWithoutOffset = k % size;

        MPI_Barrier(MPI_COMM_WORLD);
        if (offset == 0)
        {
            printf("===== k=%d =====\n", k);
        }

        // calculate u's kth row
        for (int m = k + offset; m < n; m += size)
        {
            u[k][m] = a[k][m];
            for (int j = 0; j < k; j++)
            {
                /*if (offset == 0)
                    printf("U[%d][%d] -= %fx%f\n", k, m, l[k][j], u[j][m]);//*/
                u[k][m] -= l[k][j] * u[j][m];
            }
            if (m == k && u[k][m] == 0) // check if impossible to calc
            {
                printf("Impossible to calculate as U's diagonal has at least one zero! Found at: U[%d][%d]\n", k, m);
                return 1;
            }
        }

        // share u's newly completed column
        double *currentCol = (double *)malloc(k + 1 * sizeof(double));
        if (offset == 0)
        {
            for (int j = 0; j <= k; j++)
            {
                printf("T%d(send): U[%d][%d]=%f\n", rank, j, k, u[j][k]);
                currentCol[j] = u[j][k];
            }
        }
        MPI_Bcast(currentCol, k + 1, MPI_DOUBLE, rankWithoutOffset, MPI_COMM_WORLD);
        if (offset != 0)
        {
            for (int j = 0; j <= k; j++)
            {
                //printf("T%d(recv): U[%d][%d]=%f\n", rank, j, k, currentCol[j]);
                u[j][k] = currentCol[j];
            }
        }

        MPI_Barrier(MPI_COMM_WORLD); //===========DELETE
        MPI_Barrier(MPI_COMM_WORLD); //===========DELETE

        // calculate l's kth column
        for (int i = k + offset; i < n; i += size)
        {
            l[i][k] = a[i][k];
            for (int j = 0; j < k; j++)
            {
                //printf("T%d: L[%d][%d] -= %fx%f\n", rank, i, k, l[i][j], u[j][k]);//*/
                l[i][k] -= l[i][j] * u[j][k];
            }
            l[i][k] = (double)l[i][k] / u[k][k];
        }

        // share l's newly completed row
        if (k < n - 1)
        {
            double *currentLine = (double *)malloc((k + 1) * sizeof(double));
            if (offset == 1)
            {
                for (int j = 0; j <= k; j++)
                {
                    printf("T%d(send): L[%d][%d]=%f\n", rank, k + 1, j, l[k + 1][j]);
                    currentLine[j] = l[k + 1][j];
                }
            }
            MPI_Bcast(currentLine, k+1, MPI_DOUBLE, (rankWithoutOffset + 1) % 4, MPI_COMM_WORLD);
            if (offset != 1)
                for (int j = 0; j <= k; j++)
                {
                    //printf("T%d(recv): L[%d][%d]=%f\n", rank, k + 1, j, currentLine[j]);
                    l[k + 1][j] = currentLine[j];
                }
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // ========== END ALGORITHM ==========

    double finishTime = MPI_Wtime();
    if (rank == 0)
    {
        printf("Stuff\nTime: %.4lfs\n", finishTime - startTime);
        writeMatrixToCsv("U.csv", u, n);
        writeMatrixToCsv("L.csv", l, n);
    }
    MPI_Finalize();
}