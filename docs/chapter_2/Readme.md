# Capítulo 2

Capítulo 2: 1-3, 5, 7, 10, 15-17, 19-21, 24; (13 questões)

1. When we were discussing floating point addition, we made the simplifying assumption that each of the functional units took the same amount of time. Suppose that fetch and store each take 2 nanoseconds and the remaining operations each take 1 nanosecond.

      1. How long does a floating point addition take with these assumptions ?  

      2. How long will an unpipelined addition of 1000 pairs of floats take with these assumptions ?  

      3. How long will a pipelined addition of 1000 pairs of floats take with these assumptions ?  

      4. The time required for fetch and store may vary considerably if the operands/results are stored in different levels of the memory hierarchy. Suppose that a fetch from a level 1 cache takes two nanoseconds, while a fetch from a level 2 cache takes five nanoseconds, and a fetch from main memory takes fifty nanoseconds. What happens to the pipeline when there is a level 1 cache miss on a fetch of one of the operands? What happens when there is a level 2 miss ?  

         - [Resposta questão 1](question_01/Readme.md) 


2. Explain how a queue, implemented in hardware in the CPU, could be used to improve the performance of a write-through cache.
      - [Resposta questão 2](question_02/Readme.md) 

3. Recall the example involving cache reads of a two-dimensional array (page 22). How does a larger matrix and a larger cache affect the performance of the two pairs of nested loops? What happens if MAX = 8 and the cache can store four lines ? How many misses occur in the reads of A in the first pair of nested loops ? How many misses occur in the second pair ?
     - [Resposta questão 3](question_03/Readme.md) 
4. 

5. Does the addition of cache and virtual memory to a von Neumann system change its designation as an SISD system ? What about the addition of
pipelining? Multiple issue? Hardware multithreading ? 
     - [Resposta questão 5](question_05/Readme.md) 

6. 
7. Discuss the differences in how a GPU and a vector processor might execute the following code:  
      ``` c++
         sum = 0.0;  
         for (i = 0; i < n; i++) {  
            y[i] += a∗x[i];  
            sum += z[i]∗z[i];  
         }  
      ```
      - [Resposta questão 7](question_07/Readme.md)
  
8. 
9.  
10. Suppose a program must execute 10¹² instructions in order to solve a particular problem. Suppose further that a single processor system can solve the problem in 10⁶ seconds (about 11.6 days). So, on average, the single processor system executes 10⁶ or a million instructions per second. Now suppose that the program has been parallelized for execution on a distributed-memory system. Suppose also that if the parallel program uses p processors, each processor will execute 10¹²  /p instructions and each processor must send 10⁹ ( p − 1) messages. Finally, suppose that there is no additional overhead in executing the
parallel program. That is, the program will complete after each processor has executed all of its instructions and sent all of its messages, and there won’t be any delays due to things such as waiting for messages.
    1.  Suppose it takes 10⁻⁹ seconds to send a message. How long will it take the program to run with 1000 processors, if each processor is as fast as the single processor on which the serial program was run ?  

    2. Suppose it takes 10⁻³ seconds to send a message. How long will it take the program to run with 1000 processors ?
         [Resposta questão 10](question_10/Readme.md)  

11.   
12.   
13.   
14.   
15.   
     1.  Suppose a shared-memory system uses snooping cache coherence and
write-back caches. Also suppose that core 0 has the variable x in its cache, and it executes the assignment x = 5. Finally suppose that core 1 doesn’t have x in its cache, and after core 0’s update to x, core 1 tries to execute y = x. What value will be assigned to y ? Why ?  

     2. Suppose that the shared-memory system in the previous part uses a
directory-based protocol. What value will be assigned to y ? Why ?
     3.  Can you suggest how any problems you found in the first two parts might be solved ?
         - [Resposta questão 15](question_15/Readme.md)  

16. 
     1.  Suppose the run-time of a serial program is given by $T_{serial} = n^{2}$ , where the units of the run-time are in microseconds. Suppose that a parallelization of this program has run-time $T_{parallel} = \frac{n^2}{p} + log_2(p)$. Write a program that finds the speedups and efficiencies of this program for various values of n and p. Run your program with $n = 10, 20, 40, . . . , 320$, and $p = 1, 2, 4, . . . , 128$. What happens to the speedups and efficiencies as p is increased and n is held fixed? What happens when p is fixed and n is increased ? 
     2. Suppose that $T_{parallel} = \frac{T_{serial}}{p} + T_{overhead}$ . Also suppose that we fix $p$ and increase the problem size.   
            - Show that if $T_{overhead}$ grows more slowly than $T_{serial}$ , the parallel
             efficiency will increase as we increase the problem size.
            - Show that if, on the other hand, $T_{overhead}$ grows faster than $T_{serial}$, the parallel efficiency will decrease as we increase the problem size.
        - [Resposta questão 16](question_16/Readme.md)
  
17. A parallel program that obtains a speedup greater than p—the number of processes or threads—is sometimes said to have **superlinear speedup**. However, many authors don’t count programs that overcome “resource limitations” as having superlinear speedup. For example, a program that must use secondary storage for its data when it’s run on a single processor system might be able to fit all its data into main memory when run on a large distributed-memory system. Give another example of how a program might overcome a resource limitation and obtain speedups greater than p

      - [Resposta questão 17](question_17/Readme.md)  

18. 

19. Suppose $T_{serial} = n$  and $T_{parallel} = \frac{n}{p} + log_2 (p)$, where times are in microseconds. If we increase $p$ by a factor of $k$, find a formula for how much we’ll need to increase n in order to maintain constant efficiency. How much should we increase $n$ by if we double the number of processes from 8 to 16 ? Is the parallel program scalable ?
       - [Resposta questão 19](question_19/Readme.md)  

20. Is a program that obtains linear speedup strongly scalable ? Explain your answer.
       - [Resposta questão 20](question_20/Readme.md)  

21. Bob has a program that he wants to time with two sets of data, _input_data1_ and _input_data2_. To get some idea of what to expect before adding timing functions to the code he’s interested in, he runs the program with two sets of data and the Unix shell command _time_:
      ``` shell
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
       - [Resposta questão 21](question_21/Readme.md)  
22. 
23. 
24. If you haven’t already done so in Chapter 1, try to write pseudo-code for our tree-structured global sum, which sums the elements of _loc_bin_cts_. First consider how this might be done in a shared-memory setting. Then consider how this might be done in a distributed-memory setting. In the shared-memory setting, which variables are shared and which are private ?
       - [Resposta questão 24](question_24/Readme.md)  