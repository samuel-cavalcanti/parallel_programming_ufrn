
#ifdef TBB_VERSION

#include "tbb/task_scheduler_init.h"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"
#include "tbb/cache_aligned_allocator.h"



void runWorker(std::function<void(BlockRange &, parm *)> runSimulation, parm *input, int input_size, int nThreads)
{
}

#endif // TBB_VERSION