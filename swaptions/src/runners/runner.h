#ifdef ENABLE_THREADS

#ifndef RUNNER_H
#define RUNNER_H

#include "../HJM/HJM_type.h"
#include <functional>
#include "block_range/block_range.h"

/*
    all parallel algoritms must implement the runWorker
    parm *input is a array of simulation parameters
    input size is the size of the array parm*.

*/
void runWorker(std::function<void(BlockRange &, parm *)> runSimulation, parm *input, int input_size, int nThreads);

#endif // RUNNER_H

#endif //ENABLE_THREADS