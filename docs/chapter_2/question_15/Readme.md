# Questão 15

1. Suppose a shared-memory system uses snooping cache coherence and
write-back caches. Also suppose that core 0 has the variable x in its cache, and it executes the assignment x = 5. Finally suppose that core 1 doesn’t have x in its cache, and after core 0’s update to x, core 1 tries to execute y = x. What value will be assigned to y ? Why ?  

2. Suppose that the shared-memory system in the previous part uses a
directory-based protocol. What value will be assigned to y ? Why ?

3.  Can you suggest how any problems you found in the first two parts might be solved ?


## 1 What value will be assigned to y ? Why ?

Não é possível determinar qual valor será atribuído ao y independentemente se for write-back ou write-through, uma vez que não
houve uma sincronização entre os cores sobre o valor de x. A atribuição
de y = x do core 1 pode ocorrer antes ou depois das operações no core 0.


##  shared-memory system in the previous part uses a directory-based protocol What value will be assigned to y ? Why ?

Com o sistema de arquivos, ao core 0 irá notificar o a memória principal que a consistência dos dados foi comprometida, no entanto, ainda não dá
para saber qual o valor de y, uma vez que a atribuição de y = x do core 1 pode ocorrer antes ou depois das operações no core 0.


## Can you suggest how any problems you found in the first two parts might be solved ?

Existe dois problemas, o problema da consistência do dados, temos que garantir que ambos os cores façam alterações que ambas sejam capaz de
ler e o segundo é um mecanismo de sincronização, onde por exemplo, o core 1 espera o core 0 finalizar o seu processamento com a variável x para ai sim começar o seu. Podemos utilizar por exemplo um mutex, onde
inicialmente o core 0 faria o lock e ao finalizar ele entrega a chave a qual, o core 1 pegaria.