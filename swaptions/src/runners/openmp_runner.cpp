
#ifdef OPENMP_VERSION

#ifndef OPENMP_RUNNER_HPP
#define OPENMP_RUNNER_HPP

#include "runner.h"

#include <omp.h>
#include <stdio.h>

void runWorker(std::function<void(BlockRange &, parm *)> runSimulation, parm *input, int input_size, int nThreads)
{

    printf("OpenMP is enabled !!\n");

#pragma omp parallel for num_threads(nThreads)
    for (int i = 0; i < nThreads; i++)
    {
        int thread_id = omp_get_thread_num();
        BlockRange range = find_blocked_range(thread_id, input_size, nThreads);

        runSimulation(range, input);
        printf("Call worked on thread id: %i\n", thread_id);
    }
}

#endif //OPENMP_RUNNER_HPP

#endif //OPENMP_VERSION