1. When we were discussing floating point addition, we made the simplifying assumption that each of the functional units took the same amount of time. Suppose that fetch and store each take 2 nanoseconds and the remaining operations each take 1 nanosecond.  
   a. How long does a floating point addition take with these assumptions ?  

   b. How long will an unpipelined addition of 1000 pairs of floats take with these assumptions ?  

   c. How long will a pipelined addition of 1000 pairs of floats take with these assumptions ?  

   d. The time required for fetch and store may vary considerably if the operands/results are stored in different levels of the memory hierarchy. Suppose that a fetch from a level 1 cache takes two nanoseconds, while a fetch from a level 2 cache takes five nanoseconds, and a fetch from main memory takes fifty nanoseconds. What happens to the pipeline when there is a level 1 cache miss on a fetch of one of the operands? What happens when there is a level 2 miss ?


## 1.a

| Instructions   | Time in nanosecond |
| -------------- | ------------------ |
| Fetch          | 2                  |
| Store          | 2                  |
| Functional  OP | 1                  |


"As an alternative, suppose we divide our floating point adder into seven separate pieces of hardware or functional units. The first unit will fetch two operands,
the second will compare exponents, and so on." (Página 26)

O Author do livro considera que existe sete operações, considerando que duas delas são fetch e store custa 2 nanosegundos e o restante 1 nanosegundo.

        1*5 +2*2 = 9 nanosegundos


## 1.b

Considerando que exitem 1000 pares de valores vão serem somados:

        1000*9 = 9000 nanosegundos

## 1.c

foi pensado o seguinte: Nó memento que o dado passa pelo fetch, e vai para a próxima operação já
é realizado o fetch da segunda operação. Executando o pipeline:


| Tempo em nanosegundos | Fetch | OP1  | OP2  | OP3  | OP4  | OP5  | Store |
| --------------------- | ----- | ---- | ---- | ---- | ---- | ---- | ----- |
| 0                     | 1     | wait | wait | wait | wait | wait | wait  |
| 2                     | 2     | 1    | wait | wait | wait | wait | wait  |
| 3                     | 2     | wait | 1    | wait | wait | wait | wait  |
| 4                     | 3     | 2    | wait | 1    | wait | wait | wait  |
| 5                     | 3     | wait | 2    | wait | 1    | wait | wait  |
| 6                     | 4     | 3    | wait | 2    | wait | 1    | wait  |
| 7                     | 4     | wait | 3    | wait | 2    | wait | 1     |
| 8                     | 5     | 4    | wait | 3    | wait | 2    | wait  |
| 9                     | 5     | wait | 4    | wait | 3    | wait | 2     |
| 10                    | 6     | 5    | wait | 4    | wait | 3    | 2     |
| 11                    | 6     | wait | 5    | wait | 4    | wait | 3     |


Percebe-se que a primeira instrução irá ser finalizada ou sumir na tabela quanto for 9 segundos
ou seja,a primeira instrução dura 9 segundos, no entanto, no momento em que a primeira instrução
é finalizada,a segunda já começa a ser finalizada ou seja, demora apenas 2 nanosegundos até segunda operação ser finalizada e mais 2 nanosegundos para a terceira ser finalizada e assim por diante. Por tanto para executar todos os 1000 dados, o custo total fica:
        
        9 + 999*2 = 2007

## 1.d

No caso, considerando que a cache nível não falhe a tabela continua mesma,
pois o fetch e store custam o mesmo 2 nanosegundos: 

| Tempo em nanosegundos | Fetch | OP1  | OP2  | OP3  | OP4  | OP5  | Store |
| --------------------- | ----- | ---- | ---- | ---- | ---- | ---- | ----- |
| 10                    | 6     | 5    | wait | 4    | wait | 3    | 2     |
| 11                    | 6     | wait | 5    | wait | 4    | wait | 3     |


mas se imaginarmos que na 12 iteração o Fetch e Store passa a custar 5 nanosegundos:

| Tempo em nanosegundos | Fetch | OP1  | OP2  | OP3  | OP4  | OP5  | Store |
| --------------------- | ----- | ---- | ---- | ---- | ---- | ---- | ----- |
| 10                    | 6     | 5    | wait | 4    | wait | 3    | 2     |
| 11                    | 6     | wait | 5    | wait | 4    | wait | 3     |
| 12                    | 6     | wait | wait | 5    | wait | 4    | 3     |
| 13                    | 6     | wait | wait | wait | 5    | 4    | 3     |
| 14                    | 6     | wait | wait | wait | 5    | 4    | 3     |
| 15                    | 7     | 6    | wait | wait | 5    | 4    | 3     |
| 16                    | 7     | wait | 6    | wait | wait | 5    | 4     |

Quando mais lento fica a transferência para a memória principal, mais nítido fica o gargalo de Von Neumann, ou seja, percebe-se que a performance do processador fica limitado a taxa  de transferência de dados com a memória principal.  