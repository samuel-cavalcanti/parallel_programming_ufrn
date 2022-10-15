#ifdef PTHREADS_VERSION

#ifndef PTHREAD_RUNNER_CPP
#define PTHREAD_RUNNER_CPP

#include <pthread.h>

#include "runner.h"
#include <atomic>
#include "../pascal-releases/include/pascalops.h"

typedef struct PthreadWorker
{

    std::function<void(BlockRange &, parm *)> runSimulation;
    BlockRange range;
    parm *input;
    std::atomic<int> *counter;
} PthreadWorker;

void *func(void *arg)
{
    auto worker = *((PthreadWorker *)arg);

    worker.runSimulation(worker.range, worker.input);

    worker.counter->fetch_add(1);

    return NULL;
}

void runWorker(std::function<void(BlockRange &, parm *)> runSimulation, parm *input, int input_size, int nThreads)
{
    auto threads = new pthread_t[nThreads];

    std::atomic<int> counter(0);
    pthread_attr_t pthread_custom_attr;
    pthread_attr_init(&pthread_custom_attr);
    
    pascal_start(1);
    for (auto i = 1; i < nThreads; i++)
    {
        BlockRange r = find_blocked_range(i, input_size, nThreads);
        auto workerParams = new PthreadWorker{runSimulation, r, input, &counter};
        pthread_create(&threads[i], &pthread_custom_attr, func, workerParams);
    }

    BlockRange r = find_blocked_range(0, input_size, nThreads);

    auto workerParams = new PthreadWorker{runSimulation, r, input, &counter};

    func(workerParams);

    while (counter.load() < nThreads)
    {
        /* wait */
    }
    pascal_stop(1);

    delete[] threads;
}

#endif // PTHREAD_RUNNER_CPP

#endif // PTHREADS_VERSION
