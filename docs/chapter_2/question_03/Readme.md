# Questão 3

Recall the example involving cache reads of a two-dimensional array (page 22). How does a larger matrix and a larger cache affect the performance of the two pairs of nested loops? What happens if MAX = 8 and the cache can store four lines ? How many misses occur in the reads of A in the first pair of nested loops ? How many misses occur in the second pair ?



## Exemplo envolvendo leituras na cache em um array bidimensional
```c
double A[MAX][MAX], x[MAX], y[MAX];

//. . .
// Initialize A and x, assign y = 0 ∗/
//. . .


/∗ First pair of loops ∗/
for (i = 0; i < MAX; i++)
    for (j = 0; j < MAX; j++)
        y[i] += A[i][j]∗x[j];

//. . .
// Assign y = 0 ∗/
//. . .

/∗ Second pair of loops ∗/
for (j = 0; j < MAX; j++)
    for (i = 0; i < MAX; i++)
        y[i] += A[i][j]∗x[j];
```

| Cache line | Elements of A                        |
| ---------- | ------------------------------------ |
| 0          | $A[0][0], A[0][1], A[0][2], A[0][3]$ |
| 1          | $A[1][0], A[1][1], A[1][2], A[1][3]$ |
| 2          | $A[2][0], A[2][1], A[2][2], A[2][3]$ |
| 3          | $A[3][0], A[3][1], A[3][2], A[3][3]$ |



## How does a larger matrix and a larger cache affect the performance of the two pairs of nested loops ?

Supondo que a cache tenha a mesma proporção do que a Matrix, o número de cache miss seria igual ao número de linhas da matriz, como apontado no exemplo dado no livro, quando o processador Pede o valor $A[0][0]$, baseado na ideia de vizinhança, a cache carrega todas as outras colunas da linha $0$, portanto é plausível pensar que o número de miss é igual ao número de linhas, ou seja,
o número miss é igual a __*MAX*__, pois a cache tem o mesmo número de linhas que a matrix A, suponto que não preciso me preocupar com **_x_** e **_y_**.


## if MAX = 8 and the cache can store four lines ? How many misses occur in the reads of A in the first pair of nested loops ? How many misses occur in the second pair ?


### First pair of loops

Tendo a a cache armazenando metade dos valores de uma linha da Matriz A, então
para cada linha da Matriz, vai haver duas cache miss, a primeira np $A[i][0]$ e a segunda no $A[i][4]$. Outro ponto é que como a cache só possui 4 linhas, então
após ocorrer os cache misses $A[0][0]$ ,$A[0][4]$ e $A[1][0]$, $A[1][4]$ toda a cache terá sindo preenchida, ou seja, Tento a matriz 8 linhas e para cada linha tem 2 cache miss por tanto:

$8*2 =16 \text{ cache miss}$ 

como tanto a primeira parte quando na segunda parte, percorre-se todas as linhas
irá haver 16 cache miss, suponto que não preciso me preocupar com **_x_** e **_y_**.


| Cache line | Elements of A                        |
| ---------- | ------------------------------------ |
| 0          | $A[0][0], A[0][1], A[0][2], A[0][3]$ |
| 1          | $A[0][4], A[0][5], A[0][6], A[0][7]$ |
| 2          | $A[1][0], A[1][1], A[1][2], A[1][3]$ |
| 3          | $A[1][4], A[1][5], A[1][6], A[1][7]$ |


### Second pair of loops

No segundo par de loops, vemos que o segundo laço for, percorre os valores:
$A[0][0]$, $A[1][0]$, $A[2][0]$ ... $A[7][0]$, para quando $j =0$. Isso faz com que todo hit seja miss, ou seja iremos ter miss para cada acesso em A, portanto:

$$ 8*8 = 64 \text{ cache miss} $$


```c
/∗ Second pair of loops ∗/
for (j = 0; j < MAX; j++)
    for (i = 0; i < MAX; i++)
        y[i] += A[i][j]∗x[j]; // cada acesso um miss.
```