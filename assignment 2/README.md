mpicc eratosthenesDistr.c -Wall -O3 -o eratosthenesDistr && mpirun -n 4 --use-hwthread-cpus eratosthenesDistr 137000000

gcc eratosthenes.c -Wall -O3 -o eratosthenes -fopenmp && ./eratosthenes
gcc luDecomposition.c -Wall -O3 -fopenmp -o lu && ./lu

## LU - seq
1000x1000 - 0.81s
2000x2000 - 10s
3000x3000 - 44s
4000x4000 - 129s
5000x5000 - 274s
6000x6000 - 507s

## LU - shared
1000x1000 - 1.56s
2000x2000 - 8s
3000x3000 - 24s
4000x4000 - 66s
5000x5000 - 129s
6000x6000 - 237s

## LU - distributed
1000x1000 - 0.81s
2000x2000 - 9s
3000x3000 - 39s
4000x4000 - 116s
5000x5000 - 280s
6000x6000 - 620s

## Sieve - seq
100000000 - 2.51s
200000000 - 6s
300000000 - 11s
400000000 - 15s
500000000 - 20s
600000000 - 28s

## Sieve - shared 4t c40
100000000 - 2.53s
200000000 - 6s
300000000 - 10s
400000000 - 12s
500000000 - 15s
600000000 - 19s

## Sieve - distributed
100000000 - 2.34s
200000000 - 5s
300000000 - 7s
400000000 - 10s
500000000 - 13s
600000000 - 16s