#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv)
{
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

    // ========== END ALGORITHM ==========

    double finishTime = MPI_Wtime();
    if(rank == 0)
    {
        printf("Stuff\nTime: %.4lfs\n", finishTime-startTime);
    }
    MPI_Finalize();
}