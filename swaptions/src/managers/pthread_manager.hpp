#ifndef PTHREAD_MANAGER_HPP
#define PTHREAD_MANAGER_HPP

#include <pthread.h>
#include "manager.h"

class PthreadManager : public Manager
{
private:
    pthread_t *threads;
    pthread_attr_t pthread_custom_attr;

    int nThreads;

public:
    PthreadManager(int nThreads)
    {
        this->nThreads = nThreads;
        this->threads = new pthread_t[nThreads];

        pthread_attr_init(&pthread_custom_attr);
    }
    ~PthreadManager()
    {
        delete[] threads;
    }
    void runWorker(void *(*worker)(void *)) override
    {
        int *threadIDs = new int[nThreads];

        threadIDs[0] = 0;

        for (auto i = 1; i < nThreads; i++)
        {
            threadIDs[i] = i;
            pthread_create(&threads[i], &pthread_custom_attr, worker, &threadIDs[i]);
        }

        worker(&threadIDs[0]);

        for (auto i = 1; i < nThreads; i++)
        {
            pthread_join(threads[i], NULL);
        }

        delete[] threadIDs;
    }
};

#endif