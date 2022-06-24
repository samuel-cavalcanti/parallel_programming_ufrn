# Questão 6
Suppose _comm_sz_ = 4 and suppose that **x** is a vector with $n = 14$ components.

1. How would the components of **x** be distributed among the processes in program that used a block distribution ?
   
2. How would the components of **x** be distributed among the processes in a program that used a cyclic distribution ?

3. How would the components of x be distributed among the processes in a program that used a block-cyclic distribution with blocksize $b =2$ ?

## block distribution
Sabendo que a função que calcula os intervalos da distribuição de blocos é a mesma função do [Questão 1 cap 1 ](../../chapter_1/question_1/Readme.md):
```c
struct range
{
    int first;
    int last;
};

struct range new_range_2(int thread_index, int p, int n)
{
    struct range r;

    int division = n / p;
    int rest = n % p;

    if (thread_index < rest)
    {
        r.first = thread_index * (division + 1);
        r.last = r.first + division + 1;
    }
    else
    {
        r.first = thread_index * division + rest;
        r.last = r.first + division;
    }

    return r;
}
```
criando [um script em c](questao_6.c) e o executando temos que:

```bash
gcc questao_6.c -o questao_6; ./questao_6
Process 0 First  0 Last  4 m Last - First: 4
Process 1 First  4 Last  8 m Last - First: 4
Process 2 First  8 Last  11 m Last - First: 3
Process 3 First  11 Last  14 m Last - First: 3
```

| processos |     |     |     |     |
| --------- | --- | --- | --- | --- |
| 0         | 0   | 1   | 2   | 3   |
| 1         | 4   | 5   | 6   | 7   |
| 2         | 8   | 9   | 10  |     |
| 3         | 11  | 12  | 13  |     |

## cyclic distribution
Sabendo que a cyclic distribution é a transposta da matriz, pois  é uma matriz quadrada,então:


| processos |     |     |     |     |
| --------- | --- | --- | --- | --- |
| 0         | 0   | 4   | 8   | 12  |
| 1         | 1   | 5   | 9   | 13  |
| 2         | 2   | 6   | 10  | 11  |
| 3         | 3   | 7   |     |     |

##  block-cyclic distribution

Um block-cyclic distribution de blocksize $b =2$, é um cyclic distribution, onde cada componente é um bloco de tamanho tamanho $2$. Sabendo que o tamanho do vetor e $14$, logo haverá $\frac{14}{2}$ componentes. então para encontrar a distribuição temos que:

- achar o cyclic distribution de 7 componentes virtuais
- substituir cada um dos 7 componentes virtuais em dois reais


###  cyclic distribution de 7 componentes virtuais

| processos |       |       |     |     |
| --------- | ----- | ----- | --- | --- |
| 0         | $B_1$ | $B_5$ |     |     |
| 1         | $B_2$ | $B_6$ |     |     |
| 2         | $B_3$ | $B_7$ |     |     |
| 3         | $B_4$ |       |     |     |

###  substituindo os blocos.


| processos |     |     |     |     |
| --------- | --- | --- | --- | --- |
| 0         | 0   | 1   | 8   | 9   |
| 1         | 2   | 3   | 10  | 11  |
| 2         | 4   | 5   | 12  | 13  |
| 3         | 6   | 7   |     |     |
