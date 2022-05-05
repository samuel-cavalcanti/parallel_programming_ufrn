# Lista de Execícios de Programação Concorrente e distribuída

## Capítulo 1
1. Devise formulas for the functions that calculate **_my_first_i_** and **_my_last_i_** in the global sum example. Remember that each core should be assigned roughly the same number of elements of computations in the loop. Hint: First consider the case when **_n_** is evenly divisible by **_p_**
   - [Resposta Questão 1](question_1/question_1.md) 

2. We’ve implicitly assumed that each call to **_Compute_next_value_** requires roughly the same amount of work as the other calls. How would you change your answer to the preceding question if call _i_ = _k_ requires _k_ + 1 times as much work as the call with _i_ = 0? So if the first call (_i_ = 0) requires 2 milliseconds, the second call (_i_ = 1) requires 4, the third (_i_ = 2) requires 6, and so on.
   - [Resposta Questão 2](question_2/question_2.md)

3. Try to write pseudo-code for the tree-structured global sum illustrated in
Figure 1.1. Assume the number of cores is a power of two (1, 2, 4, 8, . . . ).
   - [Resposta Questão 3](question_3/question_3.md)



## testes
Foram feitos alguns scripts para testar as soluções apresentadas
para  executa-lás:
```sh
chmod +x run_tests.sh
./run_tests.sh
```
OBS: foi utilizado gcc na versão **11.2.0** e python na versão **3.10.4**