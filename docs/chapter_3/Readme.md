# Capítulo 3

Capítulo 3: 2, 4, 6, 9, 11, 12, 13, 16, 17, 19, 20, 22, 23, 27 e 28 (16 questões);


1. 
2. Modify the trapezoidal rule so that it will correctly estimate the integral even if _comm_sz_ doesn’t evenly divide n. (You can still assume that n $\geq$ _comm_sz_.).
    - [Resposta questão 2](question_02/Readme.md) 
3. 

4. Modify the program that just prints a line of output from each process (_mpi_output.c_) so that the output is printed in process rank order: process 0s output first, then process 1s, and so on.
    - [Resposta questão 4](question_04/Readme.md)
5. 

6. Suppose _comm_sz_ = 4 and suppose that **x** is a vector with $n = 14$ components.  
    1. How would the components of **x** be distributed among the processes in program that used a block distribution ?
    2. How would the components of **x** be distributed among the processes in a program that used a cyclic distribution ?
    3. How would the components of x be distributed among the processes in a program that used a block-cyclic distribution with blocksize $b =2$ ?  
          - [Resposta questão 6](question_06/Readme.md)
7. 
8. 
9.  Write an MPI program that implements multiplication of a vector by a scalar and dot product. The user should enter two vectors and a scalar, all of which are read in by process 0 and distributed among the processes. The results are calculated and collected onto process 0, which prints them. You can assume that $n$, the order of the vectors, is evenly divisible by _comm_sz_

    - [Resposta questão 9](question_09/Readme.md)

10. 
11. Finding **prefix sums** is a generalization of global sum. Rather than simply finding the sum of $n$ values,  
$x_0 + x_1 + \cdot \cdot \cdot + x_{n-1}$  
the prefix sums are the n partial sums  
$x_0, x_0 + x_1, x_0 + x_1 + x_2,   \cdot \cdot \cdot, x_0 + x_1 \cdot \cdot \cdot + x_{n-1}$  
    1. Devise a serial algorithm for computing the $n$ prefix sums of an array with $n$ elements.
    2. Parallelize your serial algorithm for a system with $n$ processes, each of which is storing one of the _x_is_.
    3. Suppose $n = 2^k$ for some positive integer $k$. Can you devise a serial algorithm and a parallelization of the serial algorithm so that the parallel algorithm requires only $k$ communication phases ?
    4. MPI provides a collective communication function, MPI Scan, that can be used to compute prefix sums:
       ```c++
       int MPI Scan(
           void*        sendbuf_p /* in  */,
           void*        recvbuf_p /* out */,
           int          count     /* in  */,
           MPI_Datatype datatype  /* in  */,
           MPI_Op       op        /* in  */,
           MPI_Comm     comm      /* in  */);
       ```
       It operates on arrays with _count_ elements; both _sendbuf_p_ and _recvbuf_p_ should refer to blocks of _count_ elements of type _datatype_. The _op_ argument
       is the same as op for _MPI_Reduce_. Write an MPI program
       that generates a random array of _count_ elements on each
       MPI process, finds the prefix sums, and print the results.



12. An alternative to a butterfly-structured allreduce is a **ring-pass** structure. In a ring-pass, if there are $p$ processes, each process $q$ sends data to process $q + 1$, except that process $p − 1$ sends data to process $0$. This is repeated until each process has the desired result. Thus, we can implement allreduce with the following code:
    ```c++
    sum = temp val = my val;

    for (i = 1; i < p; i++) {
        MPI Sendrecv replace(&temp val, 1, MPI INT, dest,
        sendtag, source, recvtag, comm, &status);
        sum += temp val;
    }
    ```
    1. Write an MPI program that implements this algorithm for allreduce. How does its performance compare to the butterfly-structured allreduce ?
    2. Modify the MPI program you wrote in the first part so that it implements prefix sums.

13. MPI Scatter and MPI Gather have the limitation that each process must send or receive the same number of data items. When this is not the case, we must use the MPI functions MPI Gatherv and MPI Scatterv. Look at the man pages for these functions, and modify your vector sum, dot product program so that it can correctly handle the case when n isn’t evenly divisible by
_comm_sz_.

14. 
15. 

16. Suppose _comm_sz_ $= 8$ and the vector **x** $= (0, 1, 2, . . . , 15)$ has been distributed among the processes using a block distribution. Draw a diagram illustrating the steps in a butterfly implementation of allgather of **x**
17. _MPI_Type_contiguous_ can be used to build a derived datatype from a collection of contiguous elements in an array. Its syntax is
    ```c++ 
    int MPI_Type_contiguous(
                            int           count,        /* in  */
                            MPI_Datatype  old_mpi_t,    /* in  */
                            MPI_Datatype∗ new_mpi_t_p  /* out */);
    ```
    Modify the _Read_vector_ and _Print_vector_ functions so that they use an MPI datatype created by a call to _MPI_Type_contiguous_ and a _count_ argument of 1 in the calls to _MPI_Scatter_ and _MPI_Gather_.

18. 
19. _MPI_Type_indexed_ can be used to build a derived datatype from arbitrary array elements. Its syntax is
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

20. The functions _MPI_Pack_ and _MPI_Unpack_ provide an alternative to derived datatypes for grouping data. _MPI_Pack_ copies the data to be sent, one block at a time, into a user-provided buffer. The buffer can then be sent and received. After the data is received, _MPI_Unpack_ can be used to unpack it from the receive buffer. The syntax of _MPI_Pack_ is
    ```c++ 
    int MPI_Pack(
                            void*         in_buf,                    /* in  */
                            int           in_buf_count,              /* in  */
                            MPI_Datatype  datatype,                  /* in  */
                            void*         pack_buf,                  /* out */
                            int           pack_buf_sz,               /* in  */
                            int*          position_p,                /* in/out */
                            MPI_Comm      comm                       /* in */);
    ```
    We could therefore pack the input data to the trapezoidal rule program with the following code:
    ```c++
    char pack buf[100];
    int position = 0;
    MPI_Pack(&a, 1, MPI_DOUBLE, pack_buf, 100, &position, comm);
    MPI_Pack(&b, 1, MPI_DOUBLE, pack_buf, 100, &position, comm);
    MPI_Pack(&n, 1, MPI_INT, pack_buf, 100, &position, comm);
    ```
    The key is the position argument. When _MPI_Pack_ is called, position should
    refer to the first available slot in _pack_buf_. When _MPI_Pack_ returns, it refers
    to the first available slot after the data that was just packed, so after process 0
    executes this code, all the processes can call _MPI_Bcast_:
    ```c++
        MPI_Bcast(pack_buf, 100, MPI_PACKED, 0, comm);
    ```
    Note that the MPI datatype for a packed buffer is MPI_PACKED. Now the other processes can unpack the data using: _MPI_Unpack_:
    ```c++ 
    int MPI_Unpack(
                            void*         pack_buf,                  /* in  */
                            int           pack_buf_sz,               /* in  */
                            int*          position_p,                /* in/out */
                            void*         out_buf,                   /* out */
                            int           out_buf_count,             /* in  */
                            MPI_Datatype  datatype,                  /* in  */
                            MPI_Comm      comm                       /* in */);
    ```
    This can be used by “reversing” the steps in _MPI_Pack_, that is, the data is
    unpacked one block at a time starting with position = 0.
    Write another Get input function for the trapezoidal rule program. This one should use
    _MPI_Pack_ on process 0 and _MPI_Unpack_  on the other processes.

21. 

22. Time our implementation of the trapezoidal rule that uses _MPI_Reduce_. How will
    you choose n, the number of trapezoids ? How do the minimum times compare to the mean and median times ? What are the speedups ? What are the efficiencies ? On the basis of the data you collected, would you say that the trapezoidal rule is scalable ?

23. Although we don’t know the internals of the implementation of _MPI_Reduce_, we might guess that it uses a structure similar to the binary tree we discussed. If this is the case, we would expect that its run-time would grow roughly at the rate of $log_2(p)$, since there are roughly $log_2(p)$ levels in the tree. (Here, $p =$_comm_sz_.) Since the run-time of the serial trapezoidal rule is roughly proportional to $n$, the number of trapezoids, and the parallel trapezoidal rule simply applies the serial rule to $\frac{n}{p}$ trapezoids on each process, with our assumption about _MPI_Reduce_, we get a formula for the overall run-time of the parallel trapezoidal rule that looks like  
    $T_{parallel}(n,p) \approx a \times \frac{n}{p} + blog_2(p)$  
    for some constants $a$ and $b$.  
    1. Use the formula, the times you’ve taken in Exercise 3.22, and your favorite program for doing mathematical calculations to get a least-squares estimate of the values of $a$ and $b$.
    2. Comment on the quality of the predicted run-times using the formula and the values for $a$ and $b$ computed in part (a).


24. 

25. 

26. 


27. Find the speedups and efficiencies of the parallel odd-even sort. Does the program obtain linear speedups? Is it scalable ? Is it strongly scalable ? Is it weakly scalable ?

28. Modify the parallel odd-even transposition sort so that the Merge functions simply swap array pointers after finding the smallest or largest elements. What effect does this change have on the overall run-time ?


## Questão 16
Quando o compilador não é capaz de vetorizar automaticamente, ou vetoriza de forma ineficiente, o OpenMP provê a diretiva** omp simd**, com a qual o programador pode indicar um laço explicitamente para o compilador vetorizar. No código abaixo, a inclusão da cláusula reduction funciona de forma similar a flag **-ffast-math**, indicando que a redução na variável soma é segura e deve ser feita.
```c++
#pragma omp simd reduction(+:soma)
for(i=0;i<n;i++){
   x = (i+0.5)*h;
   soma += 4.0/(1.0+x*x);
}
```
Por que não é necessário usar a cláusula **private(x)** neste caso mas seria caso a diretiva **omp simd** fosse combinada com a diretiva **omp parallel for** ?

