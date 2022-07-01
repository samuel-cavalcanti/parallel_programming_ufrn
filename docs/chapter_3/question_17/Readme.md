# Questão 17

_MPI_Type_contiguous_ can be used to build a derived datatype from a collection of contiguous elements in an array. Its syntax is
```c++ 
int MPI_Type_contiguous(
                        int           count,        /* in  */
                        MPI_Datatype  old_mpi_t,    /* in  */
                        MPI_Datatype∗ new_mpi_t_p  /* out */);
```
Modify the _Read_vector_ and _Print_vector_ functions so that they use an MPI datatype created by a call to _MPI_Type_contiguous_ and a _count_ argument of 1 in the calls to _MPI_Scatter_ and _MPI_Gather_.




``` c title="question_13.c"
--8<-- "docs/chapter_3/question_17/question_17.c"
```

```bash
mpicc question_17.c -o question_17 && mpirun -n 4 ./question_17 5 "1,1,1,1,1,1" "2,2,2,2,2,2" 6 && rm question_17

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