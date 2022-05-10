# Lista de Execícios de Programação Concorrente e distribuída

## Capítulo 1
1. Devise formulas for the functions that calculate **_my_first_i_** and **_my_last_i_** in the global sum example. Remember that each core should be assigned roughly the same number of elements of computations in the loop. Hint: First consider the case when **_n_** is evenly divisible by **_p_**
   - [Resposta Questão 1](question_1/Readme.md) 

2. We’ve implicitly assumed that each call to **_Compute_next_value_** requires roughly the same amount of work as the other calls. How would you change your answer to the preceding question if call _i_ = _k_ requires _k_ + 1 times as much work as the call with _i_ = 0? So if the first call (_i_ = 0) requires 2 milliseconds, the second call (_i_ = 1) requires 4, the third (_i_ = 2) requires 6, and so on.
   - [Resposta Questão 2](question_2/Readme.md)

3. Try to write pseudo-code for the tree-structured global sum illustrated in
Figure 1.1. Assume the number of cores is a power of two (1, 2, 4, 8, . . . ).
   - [Resposta Questão 3](question_3/Readme.md)

4. As an alternative to the approach outlined in the preceding problem we can use C’s bitwise operators to implement the tree-structured global sum. In order to see how this works, it helps to write down the binary (base 2) representation of each of the core ranks, and note the pairings during each stage
   - [Resposta Questão 4](question_4/Readme.md) 

5. What happens if your pseudo-code in Exercise 1.3 or Exercise 1.4 is run when the number of cores is not a power of two (e.g., 3, 5, 6, 7) ? Can you modify the
pseudo-code so that it will work correctly regardless of the number of cores ?
   - [Resposta Questão 5](question_5/Readme.md)

6. Derive formulas for the number of receives and additions that core 0 carries out using:  
   a. the original pseudo-code for a global sum  
   b. the tree-structured global sum.  
Make a table showing the numbers of receives and additions carried out by core
0 when the two sums are used with 2, 4, 8, . . . , 1024 cores.

   - [Resposta Questão 6](question_6/Readme.md)
  

9. Write an essay describing a research problem in your major that would benefit from the use of parallel computing. Provide a rough outline of how parallelism would be used. Would you use task- or data-parallelism ?
   - [Resposta Questão 9](question_9/Readme.md)
  

## testes
Foram feitos alguns scripts para testar as soluções apresentadas
para  executa-lás:
```sh
chmod +x run_tests.sh
./run_tests.sh
```
OBS: foi utilizado gcc na versão **11.2.0** e python na versão **3.10.4**