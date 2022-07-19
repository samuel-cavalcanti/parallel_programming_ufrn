// #define OPENMP_VERSION
//
#ifdef OPENMP_VERSION

#include "find_z_values.h"
#include "../HJM.h"
#include "runners/block_range/block_range.h"
#include <omp.h>
#include <iostream>
#include <atomic>
void worker_find_z(BlockRange r, FTYPE **pdZ, FTYPE **randZ, int iN, int BLOCKSIZE)
{

    for (int i = r.begin; i < r.end; i++)
    {
        auto l = i / BLOCKSIZE;
        auto b = i % BLOCK_SIZE;

        for (int j = 1; j < iN; ++j)
        {
            pdZ[l][BLOCKSIZE * j + b] = CumNormalInv(randZ[l][BLOCKSIZE * j + b]); /* 18% of the total executition time */
        }
    }
}
void find_z_values(FTYPE **pdZ, FTYPE **randZ, int BLOCKSIZE, int iN, int iFactors)
{

    auto input_size = BLOCKSIZE * iFactors;

#pragma omp parallel for num_threads(4)
    for (int i = 0; i < input_size; i++)
    {
        auto l = i / BLOCKSIZE;
        auto b = i % BLOCK_SIZE;

        for (int j = 1; j < iN; ++j)
        {
            pdZ[l][BLOCKSIZE * j + b] = CumNormalInv(randZ[l][BLOCKSIZE * j + b]); /* 18% of the total executition time */
        }
    }
}

#endif
