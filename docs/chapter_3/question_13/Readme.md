# Questão 13

MPI Scatter and MPI Gather have the limitation that each process must send or receive the same number of data items. When this is not the case, we must use the MPI functions MPI Gatherv and MPI Scatterv. Look at the man pages for these functions, and modify your vector sum, dot product program so that it can correctly handle the case when n isn’t evenly divisible by _comm_sz_.




``` c title="question_13.c"
--8<-- "docs/chapter_3/question_13/question_13.c"
```

```bash
mpicc question_13.c -o question_13 && mpirun -n 4 ./question_13 5 "1,1,1,1,1,1" "2,2,2,2,2,2" 6 && rm question_13
                Input
vector size 6
v_1: [1.000000,1.000000,1.000000,1.000000,1.000000,1.000000]
v_2: [2.000000,2.000000,2.000000,2.000000,2.000000,2.000000]
Scalar: 5.000000
                 Result
Dot: 12.000000
scalar v1: [5.000000,5.000000,5.000000,5.000000,5.000000,5.000000]
scalar v2: [10.000000,10.000000,10.000000,10.000000,10.000000,10.000000]
```