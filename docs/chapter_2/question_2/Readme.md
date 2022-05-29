# Questão 2

Explain how a queue, implemented in hardware in the CPU, could be used to improve the performance of a write-through cache.


Como observado na [questão 1](../question_1/Readme.md) cada momento que a escrita é debilitada, fica nítido o gargalo de Von Neuman se considerarmos que uma escrita na cache é uma escrita na memória principal, então cada Store iria demorar __*50*__ nano segundos. Colocando uma fila e supondo que ela nunca fique cheia, a CPU não irá gastar tanto tempo no Store, mas uma vez a fila
cheia, a CPU terá que aguardar uma escrita na memória principal. 


## Tabela com cache miss questão 1

| Tempo em nanosegundos | Fetch | OP1  | OP2  | OP3  | OP4  | OP5  | Store |
| --------------------- | ----- | ---- | ---- | ---- | ---- | ---- | ----- |
| 10                    | 6     | 5    | wait | 4    | wait | 3    | 2     |
| 11                    | 6     | wait | 5    | wait | 4    | wait | 3     |
| 12                    | 6     | wait | wait | 5    | wait | 4    | 3     |
| 13                    | 6     | wait | wait | wait | 5    | 4    | 3     |
| 14                    | 6     | wait | wait | wait | 5    | 4    | 3     |
| 15                    | 7     | 6    | wait | wait | 5    | 4    | 3     |
| 16                    | 7     | wait | 6    | wait | wait | 5    | 4     |