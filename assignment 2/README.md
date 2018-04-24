mpicc -o main main.c #-O3
mpirun --use-hwthread-cpus -np 4 ./main