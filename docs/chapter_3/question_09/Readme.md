# Questão 9

Write an MPI program that implements multiplication of a vector by a scalar and dot product. The user should enter two vectors and a scalar, all of which are read in by process 0 and distributed among the processes. The results are calculated and collected onto process 0, which prints them. You can assume that $n$, the order of the vectors, is evenly divisible by _comm_sz_


``` c title="question_9.c"
--8<-- "docs/chapter_3/question_09/question_9.c"
```

## teste
```bash
mpirun -n 4 ./question_9 5 "1,1,1,1" "2,2,2,2" 4
                Input
vector size 4
v_1: [1.000000,1.000000,1.000000,1.000000]
v_2: [2.000000,2.000000,2.000000,2.000000]
Scalar: 5.000000
                 Result
Dot: 8.000000
scalar v1: [5.000000,5.000000,5.000000,5.000000]
scalar v2: [10.000000,10.000000,10.000000,10.000000]
```