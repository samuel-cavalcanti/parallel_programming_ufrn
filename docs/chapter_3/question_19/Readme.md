# Questão 19

_MPI_Type_indexed_ can be used to build a derived datatype from arbitrary array elements. Its syntax is
```c++ 
int MPI_Type_indexed(
                        int           count,                    /* in  */
                        int           array_of_blocklengths[],  /* in  */,
                        int           array_of_displacements[], /* in  */,  
                        MPI_Datatype  old_mpi_t,                /* in  */
                        MPI_Datatype∗ new_mpi_t_p               /* out */);
```
Unlike _MPI_Type_create_struct_, the displacements are measured in units of
_old_mpi_t_ --not bytes. Use _MPI_Type_indexed_ to create a derived datatype
that corresponds to the upper triangular part of a square matrix. For example
in the $4 \times 4$ matrix.

$$
\begin{pmatrix}
    0 & 1 & 2 & 3\\
    4 & 5 & 6 & 7\\
    8 & 9 & 10 & 11\\
    12 & 13 & 14 & 15\\ 
\end{pmatrix}
$$

the upper triangular part is the elements 0, 1, 2, 3, 5, 6, 7, 10, 11, 15. Process
0 should read in an $n \times n$ matrix as a one-dimensional array, create the derived datatype, and send the upper triangular part with a single call to _MPI_Send_. Process 1 should receive the upper triangular part with a single call ot _MPI_Recv_ and then print
the data it received.


``` c title="question_19.c"
--8<-- "docs/chapter_3/question_19/question_19.c"
```


```bash
mpicc -lm question_19.c -o question_19 && mpirun -n 2 ./question_19 "0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15" 16 && rm question_19

                Input
vector size 16
rank 4
                matrix 
[0.000000,1.000000,2.000000,3.000000]
[4.000000,5.000000,6.000000,7.000000]
[8.000000,9.000000,10.000000,11.000000]
[12.000000,13.000000,14.000000,15.000000]

                Result
upper triangular part of a square matrix: 
[0.000000,1.000000,2.000000,3.000000,5.000000,6.000000,7.000000,10.000000,11.000000,15.000000]
```
