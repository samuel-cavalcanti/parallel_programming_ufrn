#ifndef OPENMP_MANAGER_HPP
#define OPENMP_MANAGER_HPP

#include "manager.h"

#include <omp.h>
#include <stdio.h>

class OpenmpManager : public Manager
{
private:
    int nThreads;

public:
    OpenmpManager(int nThreads)
    {
        this->nThreads = nThreads;
    };
    ~OpenmpManager();

    void runWorker(void *(*worker)(void *)) override
    {

        printf("OpenMP is enabled !!\n");

#pragma omp parallel for num_threads(nThreads)
        for (int i = 0; i < nThreads; i++)
        {
            int thread_id = omp_get_thread_num();
            printf("Call worked on thread id: %i\n", thread_id);
            worker(&thread_id);
        }
    }
};

#endif