# Questão 1

Devise formulas for the functions that calculate **_my_first_i_** and **_my_last_i_** in the global sum example. Remember that each core should be assigned roughly the same number of elements of computations in the loop. Hint: First consider the case when $n$ is evenly divisible by $p$


## Observações
Nó magnânimo mundo da programação, é bastante comum
adotar a seguinte convenção na hora dividir um vetor em pedaços.
Onde o primeiro valor é o indicie que está contido no vetor e segundo valor é limite, por exemplo: 
$$ vetor= [a_0,b_1,c_2,d_3] $$
$$ vetor[1:3] == [b_1,c_2]$$
$$ vetor[1:2] == [b_1]$$
Essa será a convenção adota aqui.


Para resolver o problema primeiro, temos que pensar no caso mais simples onde $n$ é divisível pro $p$, ou seja:
$$ \frac{n}{p} = a $$
$$ n = ap $$

nesse caso, podemos dra pedaços de tamanho $a$ para cada processo.
Onde o primeiro pedaço teria o primeiro índice em $0$ e terminaria no indicie $a-1$. o segundo pedaço começaria em $a$ e terminaria em $2*a -1$. ou seja..
$$ p_0 = [0:a] $$
$$ p_1 = [a:2a] $$
$$ p_2 = [2a: 3a] $$
$$ p_n =[an: (n+1)a] $$
No entanto e se $n$ não for divisível por $p$, ou seja:
$$ n = ap + r $$
onde $r$ é o resto da divisão. Pensando de forma simples, podemos
atribuir todo o "restante" no processo $0$ e repetir a mesma ideia, ou seja, o processo $0$ teria o tamanho $a +r$ e outros processos teria o tamanho $a$.

| $p$  | tamanho |
| ---- | ------- |
| 0    | a+r     |
| 1    | a       |
| 2    | a       |
| 3    | a       |
| ..   | ...     |
| r-1  | a       |
| r    | a       |
| r +1 | a       |
| ..   | ...     |
Pensando em termos de intervalo ficaria assim:
$$ p_0 = [0:a+r] $$
$$ p_1 = [a + r: 2a +r] $$
$$ p_2 = [2a +r:3a +r] $$
$$ p_n =[an +r: (n+1)a +r] $$
Essa primeira ideia é justamente a função **new_range**. Perceba
que podemos melhorar ainda mais o algoritmo, podemos dividir $r$
para os primeiros $p_r$ cores até que não "reste" mais nada  atribuindo $+1$ no tamanho dos $p_r$ cores.
| $p$  | tamanho |
| ---- | ------- |
| 0    | a+1     |
| 1    | a+1     |
| 2    | a+1     |
| 3    | a+1     |
| ..   | ...     |
| r-1  | a+1     |
| r    | a       |
| r +1 | a       |
| ..   | ...     |

Pensando em termos de intervalo:
$$ p_0 = [0: a +1] $$
$$ p_1 = [a + 1: 2a +1] $$
$$ p_2 = [2a +1: 3a +1] $$
$$ p_{r-1} = [(r-1)a +1,(r)a +1] $$
$$ p_r = [ra +1,(r+1)a  ] $$
$$ p_{r+1} = [(r+1)a:(r+2)a] $$
$$ p_n =[an +r: (n+1)a ] $$

Essa é a função **new_range_2**. Para testar ambas as funções
foi criado o programa [question_1.c](question_1.c).



```c
struct range
{
    int first;
    int last;
};
struct range new_range(int thread_index, int p, int n)
{
    struct range r;

    int division = n / p;
    int rest = n % p;

    if (rest == 0)
    {
        r.first = thread_index * division;
        r.last = (thread_index + 1) * division;
    }
    else
    {
        r.first = thread_index == 0 ? 0 : thread_index * division + rest;
        r.last = (thread_index + 1) * division + rest;
    }

    if (r.last > n)
        r.last = n;

    return r;
}

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
Saída:
```bash
First  0 Last  20 m Last - First: 20
First  20 Last  40 m Last - First: 20
First  40 Last  60 m Last - First: 20
First  60 Last  80 m Last - First: 20
First  80 Last  100 m Last - First: 20
First  0 Last  25 m Last - First: 25
First  25 Last  50 m Last - First: 25
First  50 Last  75 m Last - First: 25
First  75 Last  99 m Last - First: 24
First  99 Last  123 m Last - First: 24
Test question 1 success
OLD new range
First  0 Last  27 m Last - First: 27
First  27 Last  51 m Last - First: 24
First  51 Last  75 m Last - First: 24
First  75 Last  99 m Last - First: 24
First  99 Last  123 m Last - First: 24
```
