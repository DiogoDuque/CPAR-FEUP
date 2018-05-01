mpicc eratosthenesDistr.c -Wall -O3 -o eratosthenesDistr && mpirun -n 4 --use-hwthread-cpus eratosthenesDistr 137000000

gcc eratosthenes.c -Wall -O3 -o eratosthenes -fopenmp && ./eratosthenes
gcc luDecomposition.c -Wall -O3 -fopenmp -o lu && ./lu
