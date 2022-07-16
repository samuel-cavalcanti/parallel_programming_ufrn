#ifndef RANGE_H
#define RANGE_H

typedef struct Range
{
    int begin;
    int end;
} Range;

Range find_blocked_range(int thread_index, int input_size, int threads)
{
    Range r;

    int division = input_size / threads;
    int rest = input_size % threads;

    if (thread_index < rest)
    {
        r.begin = thread_index * (division + 1);
        r.end = r.begin + division + 1;
    }
    else
    {
        r.begin = thread_index * division + rest;
        r.end = r.begin + division;
    }

    return r;
}

#endif