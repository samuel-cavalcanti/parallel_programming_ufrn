#ifndef ENABLE_THREADS

#include "find_z_values.h"
#include "../HJM.h"

void find_z_values(FTYPE **pdZ, FTYPE **randZ, int BLOCKSIZE, int iN, int iFactors)
{

    for (int l = 0; l < iFactors; ++l)
    {
        for (int b = 0; b < BLOCKSIZE; b++)
        {
            for (int j = 1; j < iN; ++j)
            {
                pdZ[l][BLOCKSIZE * j + b] = CumNormalInv(randZ[l][BLOCKSIZE * j + b]); /* 18% of the total executition time */
            }
        }
    }
}

#endif