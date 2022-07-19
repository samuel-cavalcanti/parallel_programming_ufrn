
#ifdef OPENMP_VERSION

#ifndef OPENMP_RUNNER_HPP
#define OPENMP_RUNNER_HPP

#include "runner.h"

#include <omp.h>
#include <stdio.h>

void runWorker(std::function<void(BlockRange &, parm *)> runSimulation, parm *input, int input_size, int nThreads)
{


#pragma omp parallel for num_threads(nThreads)
    for (int i = 0; i < nThreads; i++)
    {

        BlockRange range = find_blocked_range(i, input_size, nThreads);

        runSimulation(range, input);
       
    }
}

#endif //OPENMP_RUNNER_HPP

#endif //OPENMP_VERSION