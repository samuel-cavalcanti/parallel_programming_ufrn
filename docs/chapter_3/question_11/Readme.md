# Questão 11

Finding **prefix sums** is a generalization of global sum. Rather than simply finding the sum of $n$ values,  
$$ x_0 + x_1 + \cdot \cdot \cdot + x_{n-1} $$  
the prefix sums are the n partial sums  
$$ x_0, x_0 + x_1, x_0 + x_1 + x_2,   \cdot \cdot \cdot, x_0 + x_1 \cdot \cdot \cdot + x_{n-1} $$  
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


## Prefix sums serial

``` c++ title="question_11_serial.cpp"
--8<-- "docs/chapter_3/question_11/question_11_serial.cpp"
```

```bash
c++ question_11_serial.cpp -o question_11_serial 
./question_11_serial 
Passed in tests
```

## Prefix sums MPI 
A figura a baixo, é um exemplo onde
a entrada é é um vetor $v = [1,2,3,4]$ e o número de processos
é igual a 4. Logo cada processo terá apenas um número. A ideia 
é focar só na transferência dos valores entre os processos.
Perceba que o processo raiz apenas envia o seu valor para o
próximo nó e que o último processo apenas recebe os valor
do nó anterior e que o número de sincronizações é $p-1$, onde
$p$ é um números de processos, no exemplo $p = 4$, por tanto
o número de sincronizações é $3$.

![](linear.png)


``` c title="question_11_mpi.c"
--8<-- "docs/chapter_3/question_11/question_11_mpi.c"
```


```bash
mpicc question_11_mpi.c -o question_11_mpi
mpiexec -n 4 ./question_11_mpi 8 1,2,3,4,5,6,7,8

                Input
vector size 8
vector input: [1.000000,2.000000,3.000000,4.000000,5.000000,6.000000,7.000000,8.000000]
                Result
Prefix sum: [1.000000,3.000000,6.000000,10.000000,15.000000,21.000000,28.000000,36.000000]
```


## Prefix sums MPI butterfly structured

Nó caso para reduzirmos o número de sincronizações, temos que
utilizar a estrutura butterfly, idealmente nós não deveríamos
implementar a estrutura, uma vez que **implementar** ela é
de forma ótima é responsabilidade do próprio MPI. Deveríamos apenas utilizar e passar os parâmetros
adequados. - "Alternatively, we might have the processes exchange
partial results instead of using one-way communications. Such a communication pattern is sometimes called a butterfly (see Figure 3.9). Once again, we don’t want to have to decide on which structure to use, or how to code it for optimal performance."
Perceba que utilizando a estrutura butterfly atente os requisitos.
$n =4$, implica o número de sincronizações é $2$ e caso tenha número de cores suficiente, quando $n =8$, o número de   número de sincronizações é $3$, ou seja, $n = 2^{k}$.
![](butterfly.png)


``` c title="question_11_butterfly.c"
--8<-- "docs/chapter_3/question_11/question_11_butterfly.c"
```

```bash
mpicc question_11_butterfly.c -o question_11_butterfly
mpiexec -n 4 ./question_11_butterfly 8 1,2,3,4,5,6,7,8

                Input
vector size 8
vector input: [1.000000,2.000000,3.000000,4.000000,5.000000,6.000000,7.000000,8.000000]
                Result
Prefix sum: [1.000000,3.000000,6.000000,10.000000,15.000000,21.000000,28.000000,36.000000]
```

## Prefix sum SCAN function

``` c title="question_11_scan.c"
--8<-- "docs/chapter_3/question_11/question_11_scan.c"
```

```bash
mpicc question_11_scan.c -o question_11_scan
mpiexec -n 4 ./question_11_scan 8 1,2,3,4,5,6,7,8 
                Input
vector size 8
vector input: [1.000000,2.000000,3.000000,4.000000,5.000000,6.000000,7.000000,8.000000]
                Result
Prefix sum: [1.000000,3.000000,6.000000,10.000000,15.000000,21.000000,28.000000,36.000000]
```
