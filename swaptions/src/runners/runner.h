#ifdef ENABLE_THREADS

#ifndef RUNNER_H
#define RUNNER_H

#include "../HJM_type.h"
#include <functional>
#include "block_range/block_range.h"

void runWorker(std::function<void(BlockRange &, parm *)> runSimulation, parm *input, int input_size, int nThreads);

#endif // RUNNER_H

#endif //ENABLE_THREADS