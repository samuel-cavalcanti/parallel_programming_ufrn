# Lista de Execícios de Programação Concorrente e distribuída

## Capítulo 1

1. Devise formulas for the functions that calculate **_my_first_i_** and **_my_last_i_** in the global sum example. Remember that each core should be assigned roughly the same number of elements of computations in the loop. Hint: First consider the case when **_n_** is evenly divisible by **_p_**

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