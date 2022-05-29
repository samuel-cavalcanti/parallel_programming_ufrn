# Questão 1

 Devise formulas for the functions that calculate **_my_first_i_** and **_my_last_i_** in the global sum example. Remember that each core should be assigned roughly the same number of elements of computations in the loop. Hint: First consider the case when **_n_** is evenly divisible by **_p_**

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
```
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
