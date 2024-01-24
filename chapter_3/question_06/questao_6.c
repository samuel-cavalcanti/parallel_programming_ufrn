#include <stdint.h>
#include <stdio.h>

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

void print_range(struct range r)
{
    printf("First  %i Last  %i m Last - First: %i\n", r.first, r.last, r.last - r.first);
}

int main(int argc, char const *argv[])
{
    size_t p = 4;
    size_t n = 14;
    size_t blocksize = 2;

    for (size_t i = 0; i < p; i++)
    {
        struct range r = new_range_2(i, p, n);
        printf("Process %i ", i);
        print_range(r);
    }

    printf("calculando ranges com %i componentes\n",n/blocksize);

    for (size_t i = 0; i < p; i++)
    {
        struct range r = new_range_2(i, p, n / blocksize);
        printf("Process %i ", i);
        print_range(r);
    }

    return 0;
}
