#include <stdio.h>

struct range
{
    int first;
    int last;
};
void print_range();

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

int test_rest_division_zero();
int test_rest_division_not_zero();
int test_old_range();
int test_range(int p, int n, struct range ranges[]);
int DEBUG = 0;

int main(int argc, char const *argv[])
{

    if (test_rest_division_zero() && test_rest_division_not_zero())
    {
        printf("Test question 1 success\n");
    }

    if (DEBUG)
    {
        printf("OLD new range\n");
        test_old_range();
    }

    return 1;
}

int test_rest_division_zero()
{
    int p = 5;
    int n = 100;
    struct range r;
    struct range ranges[5] = {
        {0, 20},
        {20, 40},
        {40, 60},
        {60, 80},
        {80, n}};

    return test_range(p, n, ranges);
}

int test_rest_division_not_zero()
{
    int p = 5;
    int n = 123;
    struct range r;
    struct range ranges[5] = {
        {0, 25},
        {25, 50},
        {50, 75},
        {75, 99},
        {99, n}};

    return test_range(p, n, ranges);
}

int test_old_range()
{
    int p = 5;
    int n = 123;
    struct range r;
    struct range ranges[5] = {
        {0, 25},
        {25, 50},
        {50, 75},
        {75, 99},
        {99, n}};

    for (int thread_index = 0; thread_index < p; thread_index++)
    {
        struct range r = new_range(thread_index, p, n);
        if (DEBUG)
            print_range(r);
    }
}

int test_range(int p, int n, struct range ranges[5])
{
    for (int thread_index = 0; thread_index < p; thread_index++)
    {
        struct range r = new_range_2(thread_index, p, n);
        if (DEBUG)
            print_range(r);

        if (r.first != ranges[thread_index].first || r.last != ranges[thread_index].last)
        {
            printf("Error on thread index: %i", thread_index);
            return 0;
        }
    }

    return 1;
}

void print_range(struct range r)
{
    printf("First  %i Last  %i m Last - First: %i\n", r.first, r.last, r.last - r.first);
}