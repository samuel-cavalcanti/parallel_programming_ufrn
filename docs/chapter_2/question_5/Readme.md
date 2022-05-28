5. Does the addition of cache and virtual memory to a von Neumann system change its designation as an SISD system ? What about the addition of
pipelining? Multiple issue? Hardware multithreading ? 


Um SISD system ou Single Instruction Single Data system, são sistemas que executam uma única instrução por vez e sua taxa de transferência de dados é
de um item por vez também. 


## the addition of cache and virtual memory

Adicionar um cache e memória virtual, pode ajudar a reduzir o tempo que __única__ instrução é lida da memória principal, mas não aumenta o número de
instruções buscadas na operação Fetch ou na Operação Store, por tanto o sistema
continua sendo Single Instruction Single Data.

## the addition of pipelining

Como demostrado na [questão 1](../question_1/Readme.md), ao adicionar um pipeline, podemos realizar a mesma instrução complexa em múltiplos dados,
ou seja, Single Instruction Multiple Data System, portanto sim.

## the addition of Multiple issue and Hardware multithreading

No momento em que possibilitamos uma máquina executar antecipadamente uma instrução ou possibilitamos a execução de múltiplas threads, nesse momento então a máquina está executando várias instruções em vários dados ao mesmo tempo, por tanto o sistema se torna Multiple Instruction Multiple Data system, ou seja, a designação muda.