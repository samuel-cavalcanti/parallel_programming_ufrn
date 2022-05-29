# Questão 17

A parallel program that obtains a speedup greater than p—the number of processes or threads—is sometimes said to have **superlinear speedup**. However, many authors don’t count programs that overcome “resource limitations” as having superlinear speedup. For example, a program that must use secondary storage for its data when it’s run on a single processor system might be able to fit all its data into main memory when run on a large distributed-memory system. Give another example of how a program might overcome a resource limitation and obtain speedups greater than p

## HTTP server em ESP32

Sistemas embarcados com  CPUs multi-core. Atualmente já existem microcontroladores como **ESP32** que dependendo do modelo pode possuir
mais de um núcleo de processamento. Sabendo que todo o programa fica carregado na memória, então uma aplicação como um servidor HTTP, pode
ter mais que o dobro de performance, quando observado o seu requests por segundo.

Fazemos o seguinte cenário:

Temos um desenvolvedor que sabe que executar operações de escrita em hardware é uma operação insegura e utiliza estruturas de
dados para sincronização dessas operações, cada dispositivo tem seu tempo de sincronização. Temos que o dispositivo A custa 5 milissegundos,
e dispositivo B custa 4 milissegundos. Sabendo que se criarmos a aplicação em single core,  temos que esperar a sincronização de A e B e que
modificar o tempo de sincronização de um dispositivo custa 3 milissegundos. Dado que se tem 2 cores, 2 conversores AD, se delegarmos cada dispositivo
para um core, eliminaremos 3 milissegundos do processo de escrita no seu pior cenário. Supondo que o tempo de um request fique 30% na escrita de
um dispositivo e que ele gasta em outras operações **8 milissegundos** temos dois cenários,

### Cenário 1

$R_{e}=0.7(8) + 0.3(5+3)$

$R_{e}=8$, ou seja uma Request de escrita custa 8 milissegundos

### Cenário 2

$R_{e}=0.7(8) + 0.3(5)$

$R_{e}=7.1$, ou seja uma Request de escrita custa 7.1 milissegundos, além do fato do **ESP32** ser capaz de realizar o dobro de requests

