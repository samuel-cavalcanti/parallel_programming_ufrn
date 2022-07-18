// #define OPENMP_VERSION
//
#ifdef OPENMP_VERSION

#include "find_z_values.h"
#include "../HJM.h"
#include "runners/block_range/block_range.h"

void find_z_values(FTYPE **pdZ, FTYPE **randZ, int BLOCKSIZE, int iN, int iFactors)
{

    for (int l = 0; l <= iFactors - 1; ++l)
    {
        for (int b = 0; b < BLOCKSIZE; b++)
        {
            for (int j = 1; j <= iN - 1; ++j)
            {
                pdZ[l][BLOCKSIZE * j + b] = CumNormalInv(randZ[l][BLOCKSIZE * j + b]); /* 18% of the total executition time */
            }
        }
    }
}

#endif
