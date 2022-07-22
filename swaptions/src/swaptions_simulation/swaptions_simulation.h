

#ifndef SWAPTIONS_SIMULATION_H
#define SWAPTIONS_SIMULATION_H

#include "HJM/HJM_type.h"

void run_swaptions_simulation(parm *  swaptions, int nSwaptions, int simulation_trials, int nThreads, long seed);

#endif // SWAPTIONS_SIMULATION_H