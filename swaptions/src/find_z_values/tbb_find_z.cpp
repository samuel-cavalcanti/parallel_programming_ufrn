#ifdef TBB_VERSION
// TBB version is not work.
#include <pthread.h>
#include "tbb/task_scheduler_init.h"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"
#include "tbb/cache_aligned_allocator.h"

#define PARALLEL_B_GRAINSIZE 8

struct ParallelB
{
    __volatile__ int l;
    FTYPE **pdZ;
    FTYPE **randZ;
    int BLOCKSIZE;
    int iN;

    ParallelB(FTYPE **pdZ_, FTYPE **randZ_, int BLOCKSIZE_, int iN_) //:
    //    pdZ(pdZ_), randZ(randZ_), BLOCKSIZE(BLOCKSIZE_), iN(iN_)
    {
        pdZ = pdZ_;
        randZ = randZ_;
        BLOCKSIZE = BLOCKSIZE_;
        iN = iN_;
        /*fprintf(stderr,"(Construction object) pdZ=0x%08x, randZ=0x%08x\n",
          pdZ, randZ);*/
    }
    void set_l(int l_) { l = l_; }

    void operator()(const tbb::blocked_range<int> &range) const
    {
        int begin = range.begin();
        int end = range.end();
        int b, j;
        /*fprintf(stderr,"B: Thread %d from %d to %d. l=%d pdZ=0x%08x, BLOCKSIZE=%d, iN=%d\n",
          pthread_self(), begin, end, l,(int)pdZ,BLOCKSIZE,iN); */

        for (b = begin; b != end; b++)
        {
            for (j = 1; j <= iN - 1; ++j)
            {
                pdZ[l][BLOCKSIZE * j + b] = CumNormalInv(randZ[l][BLOCKSIZE * j + b]); /* 18% of the total executition time */
                                                                                       // fprintf(stderr,"%d (%d, %d): [%d][%d]=%e\n",pthread_self(), begin, end,  l,BLOCKSIZE*j+b,pdZ[l][BLOCKSIZE*j + b]);
            }
        }
    }
};

void find_z_values(FTYPE **pdZ, FTYPE **randZ, int BLOCKSIZE, int iN, int iFactors)
{

    ParallelB B(pdZ, randZ, BLOCKSIZE, iN);
    for (l = 0; l <= iFactors - 1; ++l)
    {
        B.set_l(l);
        tbb::parallel_for(tbb::blocked_range<int>(0, BLOCKSIZE, PARALLEL_B_GRAINSIZE), B);
    }
}

#endif // TBB_VERSION