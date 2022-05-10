## Capítulo 5

Capítulo 5: 2-3, 5-6, 8-10, 12-16 (12 questões)

1. 
2. Download _omp_trap_1.c_ from the book’s website, and delete the critical directive. Now compile and run the program with more and more threads and larger and larger values of n. How many threads and how many trapezoids does it take before the result is incorrect ?
3. Modify _omp_trap_1.c_ so that
   1. it uses the first block of code on page 222, and
   2. the time used by the parallel block is timed using the OpenMP function _omp_get_wtime()_. The syntax is:
        ```c
            double omp get wtime(void)
        ``` 
    It returns the number of seconds that have passed since some time in the past. For details on taking timings, see Section 2.6.4. Also recall that OpenMP has a barrier directive It returns the number of seconds that have passed since some time in the past. For details on taking timings, see Section 2.6.4. Also recall that OpenMP has a barrier directive:

    ```c
    #pragma omp barrier
    ```
    Now find a system with at least two cores and time the program with

    3. one thread and a large value of n, and
    4. two threads and the same value of n. What happens ? Download _omp_trap_2.c_ from the book’s website. How does its performance compare ? Explain your answers.
4. 
5. Suppose that on the amazing Bleeblon computer, variables with type float can store three decimal digits. Also suppose that the Bleeblon’s floating point registers can store four decimal digits, and that after any floating point operation, the result is rounded to three decimal digits before being stored. Now suppose a C program declares an array a as follows:
    ```c
    float a[] = {4.0, 3.0, 3.0, 1000.0};
    ```

   1. What is the output of the following block of code if it’s run on the Bleeblon ?
        ```c
        int i;
        float sum = 0.0;

        for (i = 0; i < 4; i++)
            sum += a[i];

        printf("sum = %4.1f\n", sum);
        ```
    2. Now consider the following code:
        ```c
        int i;
        float sum = 0.0;

        #pragma omp parallel for num threads(2) \
        reduction(+:sum)
        for (i = 0; i < 4; i++)
            sum += a[i];

        printf("sum = %4.1f\n", sum);
        ```
        Suppose that the run-time system assigns iterations i = 0, 1 to thread 0 and i = 2, 3 to thread 1. What is the output of this code on the Bleeblon ?

6. Write an OpenMP program that determines the default scheduling of parallel for loops. Its input should be the number of iterations, and its output should be which iterations of a parallelized for loop are executed by which thread. For example, if there are two threads and four iterations, the output might be:
    ```shell
    Thread 0: Iterations 0 −− 1
    Thread 1: Iterations 2 −− 3
    ```
7. 
8. Consider the loop
    ```c
    a[0] = 0;
    for (i = 1; i < n; i++)
        a[i] = a[i−1] + i;
    ```
    There’s clearly a loop-carried dependence, as the value of a[i] can’t be computed without the value of a[i−1]. Can you see a way to eliminate this dependence and parallelize the loop ?
9.  Modify the trapezoidal rule program that uses a _parallel_ for directive (_omp_trap_3.c_) so that the _parallel_ for is modified by a _schedule(runtime)_ clause. Run the program with various assignments to the environment variable _OMP_SCHEDULE_ and determine which iterations are assigned to which thread. This can be done by allocating an array _iterations_ of n ints and in the _Trap_ function assigning _omp_get_thread_num()_ to _iterations[i]_ in the ith iteration of the for loop. What is the default assignment of iterations on your system ? How are _guided_ schedules determined ?
10. b