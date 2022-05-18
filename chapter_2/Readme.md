## Capítulo 2

Capítulo 2: 1-3, 5, 7, 10, 15-17, 19-21, 24; (13 questões)

1. When we were discussing floating point addition, we made the simplifying assumption that each of the functional units took the same amount of time. Suppose that fetch and store each take 2 nanoseconds and the remaining operations each take 1 nanosecond.  
   a. How long does a floating point addition take with these assumptions ?  

   b. How long will an unpipelined addition of 1000 pairs of floats take with these assumptions ?  

   c. How long will a pipelined addition of 1000 pairs of floats take with these assumptions ?  

   d. The time required for fetch and store may vary considerably if the operands/results are stored in different levels of the memory hierarchy. Suppose that a fetch from a level 1 cache takes two nanoseconds, while a fetch from a level 2 cache takes five nanoseconds, and a fetch from main memory takes fifty nanoseconds. What happens to the pipeline when there is a level 1 cache miss on a fetch of one of the operands? What happens when there is a level 2 miss ?
   - [Resposta questão 1](question_1/Readme.md) 

2. Explain how a queue, implemented in hardware in the CPU, could be used to improve the performance of a write-through cache.
3. Recall the example involving cache reads of a two-dimensional array (page 22). How does a larger matrix and a larger cache affect the performance of the two pairs of nested loops? What happens if MAX = 8 and the cache can store four lines ? How many misses occur in the reads of A in the first pair of nested loops ? How many misses occur in the second pair ?
4. 

5. Does the addition of cache and virtual memory to a von Neumann sys-
tem change its designation as an SISD system ? What about the addition of
pipelining? Multiple issue? Hardware multithreading ? 

6. 
7. Discuss the differences in how a GPU and a vector processor might execute the following code:
```c++
sum = 0.0;
for (i = 0; i < n; i++) {
   y[i] += a∗x[i];
   sum += z[i]∗z[i];
}
```
8. 
9. 
10. Suppose a program must execute 1012 instructions in order to solve a particular problem. Suppose further that a single processor system can solve the problem in 106 seconds (about 11.6 days). So, on average, the single processor system executes 106 or a million instructions per second. Now suppose that the program has been parallelized for execution on a distributed-memory system. Suppose also that if the parallel program uses p processors, each processor will execute 1012 /p instructions and each processor must send 109 ( p − 1) messages. Finally, suppose that there is no additional overhead in executing the
parallel program. That is, the program will complete after each processor has executed all of its instructions and sent all of its messages, and there won’t be any delays due to things such as waiting for messages.  
    1.  Suppose it takes 10−9 seconds to send a message. How long will it take the program to run with 1000 processors, if each processor is as fast as the single processor on which the serial program was run ?  

    2. Suppose it takes 10−3 seconds to send a message. How long will it take the program to run with 1000 processors ?

11. 
12. 
13. 
14. 
15. 1. Suppose a shared-memory system uses snooping cache coherence and
write-back caches. Also suppose that core 0 has the variable x in its cache, and it executes the assignment x = 5. Finally suppose that core 1 doesn’t have x in its cache, and after core 0’s update to x, core 1 tries to execute y = x. What value will be assigned to y ? Why ?  

    2. Suppose that the shared-memory system in the previous part uses a
directory-based protocol. What value will be assigned to y ? Why ?
    3.  Can you suggest how any problems you found in the first two parts might be solved ?

16.  
       1. Suppose the run-time of a serial program is given by T_serial = n2 , where the units of the run-time are in microseconds. Suppose that a parallelization of this program has run-time T_parallel = n2 /p + log2 (p). Write a program that finds the speedups and efficiencies of this program for various values of n and p. Run your program with n = 10, 20, 40, . . . , 320, and p = 1, 2, 4, . . . , 128. What happens to the speedups and efficiencies as p is increased and n is held fixed? What happens when p is fixed and n is increased ?
       1. Suppose that T_parallel = T_serial /p + T_overhead . Also suppose that we fix p and increase the problem size.
          - Show that if T_overhead grows more slowly than T_serial , the parallel
          efficiency will increase as we increase the problem size.
          - Show that if, on the other hand, T_overhead grows faster than T_serial , the parallel efficiency will decrease as we increase the problem size.
17. 
18. 
19. Suppose T_serial = n and T_parallel = n/p + log2 (p), where times are in microseconds. If we increase p by a factor of k, find a formula for how much we’ll need to increase n in order to maintain constant efficiency. How much should we increase n by if we double the number of processes from 8 to 16 ? Is the parallel program scalable ?
20. Is a program that obtains linear speedup strongly scalable ? Explain your answer.
21. Bob has a program that he wants to time with two sets of data, _input_data1_ and _input_data2_. To get some idea of what to expect before adding timing functions to the code he’s interested in, he runs the program with two sets of data and the Unix shell command _time_:
      ```shell
      $ time ./bobs prog < input data1
      real 0m0.001s
      user 0m0.001s
      sys 0m0.000s
      $ time ./bobs prog < input data2
      real 1m1.234s
      user 1m0.001s
      sys 0m0.111s
      ```
      The timer function Bob is using has millisecond resolution. Should Bob use it to time his program with the first set of data ? What about the second set of data ? Why or why not ?

22. 
23. 
24. If you haven’t already done so in Chapter 1, try to write pseudo-code for our tree-structured global sum, which sums the elements of _loc_bin_cts_. First consider how this might be done in a shared-memory setting. Then consider how this might be done in a distributed-memory setting. In the shared-memory setting, which variables are shared and which are private ?