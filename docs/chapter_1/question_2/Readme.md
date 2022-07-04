# Questão 2

We’ve implicitly assumed that each call to **_Compute_next_value_** requires roughly the same amount of work as the other calls. How would you change your answer to the preceding question if call _i_ = _k_ requires _k_ + 1 times as much work as the call with _i_ = 0? So if the first call (_i_ = 0) requires 2 milliseconds, the second call (_i_ = 1) requires 4, the third (_i_ = 2) requires 6, and so on.

Sabendo que desenvolvemos um algoritmo muito bom na [questão 1](../question_1/Readme.md) que busca distribuir da forma mais igual possível. Uma solução possível para esse problema seria mapear 
esse problema para o problema anterior através da soma de gauss.

## Somatório de Gauss
![Somatório de Gauss](gauss_summation.png)


Exemplo,  Supondo que $k = 10$, temos o seguinte vetor de índice:  
*indices* $= [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]$,
utilizando a lógica da somatório de gauss e organizando os indicies temos um array normalizado:  
*normalized_array* $=   [ [0, 9], [1, 8], [2, 7], [3, 6], [4, 5] ]$ 
onde o custo  de cada índice do *normalized_array* é igual, por tanto podemos usar o algoritmo da questão 1 aplicado ao *normalized_array*
resultando:

| Thread | normalized_array |
| ------ | ---------------- |
| 1      | 0,1              |
| 2      | 2,3              |
| 3      | 4                |


| Thread | Compute_next_value | cost |
| ------ | ------------------ | ---- |
| 1      | 0, 9,1, 8          | 44   |
| 2      | 2, 7,3, 6          | 44   |
| 3      | 4, 5               | 22   |


se consideramos que $\frac{k}{2}$ é divisível pro $p$ o resultados são ainda melhores, como $k = 16$ e $p =4$


| Thread | Compute_next_value | cost |
| ------ | ------------------ | ---- |
| 1      | 0, 15, 1, 14       | 68   |
| 2      | 2, 13, 3, 12       | 68   |
| 3      | 4, 11, 5, 10       | 68   |
| 4      | 6, 9, 7, 8         | 68   |
