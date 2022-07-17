#ifndef BLOCK_RANGE_H
#define BLOCK_RANGE_H

typedef struct BlockRange
{
    int begin;
    int end;
} BlockRange;

BlockRange find_blocked_range(int thread_index, int input_size, int threads);

#endif // BLOCK_RANGE_H