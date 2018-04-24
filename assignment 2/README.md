mpicc -o main main.c #-O3
mpirun --use-hwthread-cpus -np 4 ./main


gcc eratosthenes.c -Wall -O3 -o eratosthenes /opt/papi/lib/libpapi.a && ./eratosthenes
gcc luDecomposition.c -Wall -O3 -o lu /opt/papi/lib/libpapi.a && ./lu
