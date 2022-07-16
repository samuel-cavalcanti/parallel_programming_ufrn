#ifndef RUNNER_H
#define RUNNER_H

#include "../HJM_type.h"

#include <functional>
#include "range.h"

class Manager
{
private:
    /* data */
public:
    virtual void runWorker(void * (*worker) (void *) ) = 0;
};

#endif // RUNNER_H