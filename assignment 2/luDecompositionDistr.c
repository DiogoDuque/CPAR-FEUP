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
        // calculate u's kth row
        int offset = (rank - k + size) % size;
        for (int m = k + offset; m < n; m += size)
        {
            u[k][m] = a[k][m];
            for (int j = 0; j < k; j++)
            {
                u[k][m] -= l[k][j] * u[j][m];
            }
        }

        // share u's newly completed column
        int *currentCol = (int *)malloc(k + 1 * sizeof(double));
        if (offset == 0)
        {
            for (int j = 0; j <= k; j++)
            {
                currentCol[j] = u[j][k];
            }
        }
        MPI_Bcast(&currentCol, k + 1, MPI_DOUBLE, offset, MPI_COMM_WORLD);
        if (offset != 0)
            for (int j = 0; j < k + 1; j++)
            {
                u[j][k] = currentCol[j];
            }

        // calculate l's kth column
        for (int i = k + offset; i < n; i += size)
        {
            l[i][k] = a[i][k];
            for (int j = 0; j < k; j++)
            {
                l[i][k] -= l[i][j] * u[j][k];
            }
            l[i][k] /= u[k][k];
        }

        // share l's newly completed row
        int *currentLine = (int *)malloc(k + 1 * sizeof(double));
        if (offset == 0)
        {
            for (int j = 0; j < k; j++)
            {
                currentLine[j] = l[k][j];
            }
        }
        if (k != 0)
            MPI_Bcast(&currentLine, k, MPI_DOUBLE, offset, MPI_COMM_WORLD);
        if (offset != 0 && k != 0)
            for (int j = 0; j < k; j++)
            {
                l[k][j] = currentLine[j];
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