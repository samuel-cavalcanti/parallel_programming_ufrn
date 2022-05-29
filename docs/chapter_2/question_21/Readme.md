# Questão 21

Bob has a program that he wants to time with two sets of data, _input_data1_ and _input_data2_. To get some idea of what to expect before adding timing functions to the code he’s interested in, he runs the program with two sets of data and the Unix shell command _time_:
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



Segundo a referência o commando time, retorna três valores:  

- real, que o tempo total desde a inicialização até a terminação do programa
- user, o total de cpu usado pelo programa em modo usuário, ou seja,o a quantidade de cpu usada, eliminando chamadas do sistema e o tempo que o sistema ficou bloqueado ou aguardando outros processos.
- Sys, o tempo de cpu usado pelo kernel/ SO para esse processo em específico.

## Primeiro timer
Na primeira chamada observamos que o tempo coletado é praticamente 0, ou seja, o tempo levado para executar o programa esta fora da resolução do relógio do sistema, por tanto não podemos concluir nada sobre a primeira chamada e se essa for a primeira chamada é bem provável que a próxima também dê praticamente 0, uma vez que a aplicação pode ter pouco tamanho
de entrada se comparado a máquina em questão. 

## Segundo timer
Já no segundo timer podemos observar informações sobre a aplicação, estão dentro da resolução do relógio e que maior parte da aplicação foi gasta em  em user mode. Bob pode fazer proveito dessas informações.