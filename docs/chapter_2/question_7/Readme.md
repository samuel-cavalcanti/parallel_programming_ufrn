7. Discuss the differences in how a GPU and a vector processor might execute the following code:
```c++
sum = 0.0;
for (i = 0; i < n; i++) {
   y[i] += a∗x[i];
   sum += z[i]∗z[i];
}
```
## CPU com vetorização
Um processo de vetorização em cima desse laço for dividiria as entradas
em chucks ou blocos de dados e executaria em paralelo a instrução complexa.
Algo como:

```c
//executando o bloco paralelamente.
y[0] += a∗x[0];
y[1] += a∗x[1];
y[2] += a∗x[2];
y[3] += a∗x[3];

z[0]∗z[0] // executando em paralelo
z[1]∗z[1] // executando em paralelo
z[2]∗z[2] // executando em paralelo
z[3]∗z[3] // executando em paralelo
// somando tudo depois
sum+=  z[0]∗z[0] + z[1]∗z[1] + z[2]∗z[2] + z[3]∗z[3]; 
```

## GPU

Atualmente essa operação em GPU é muito mais interessante, pois hoje podemos
compilar ou gerar instruções complexas que podem ser executas em GPU. A primeira vantagem seria separar o calculo do sum:

        sum += z[i]∗z[i];

do calculo do  **_y_**

          y[i] += a∗x[i];

ficando assim:

### Shaders glsl equivalente suponto que n = 4

```glsl
# version 330

layout(location = 0) in vec4 x;
layout(location = 1) in mat4 a;
layout(location = 2) in vec4 y; 

/* o buffer gl_Position é ré-inserido no y */
void main()
{
    gl_Position =  a*x + y; 
}
//
```

```glsl
# version 330

layout(location = 0) in mat4 z; 
uniform float sum; 

/* transpose é uma função que calcula a transposta já existem no Glsl */
void main()
{   
    mat4 temp =  transpose(z) * z;

    sum = 0; 
     for (int i = 0; i < 4; i++) 
     // desde que o laço for seja baseado em constantes ou variáveis uniforms
     // esse laço for é possível.
    {
        sum += temp[i];
    }

    // recupera o valor no index 0
    gl_Position = vec4(sum, 0.0, 0.0, 0.0, 0.0); 
}
```
A grande vantagem de usar os shaders seria dependendo do tamanho do vetor de dados,  executar as instruções de uma só vez em todos os dados, na prática
assim como a vetorização envia em blocos, na GPU você  também enviaria em blocos, comumente chamados de buffers, a grande diferença seria justamente no fato que um bloco na GPU possui um tamanho muito maior que o bloco da vetorização.