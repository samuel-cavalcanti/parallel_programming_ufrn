// HJM_Securities.cpp
// Routines to compute various security prices using HJM framework (via Simulation).
// Authors: Mark Broadie, Jatin Dewanwala
// Collaborator: Mikhail Smelyanskiy, Jike Chong, Intel
// Modified by Christian Bienia for the PARSEC Benchmark Suite

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "nr_routines.h"
#include "HJM.h"
#include "HJM_Securities.h"
#include "HJM_type.h"

// C++
#include <iostream>
#include <fstream>
#include <sstream>
// #define ENABLE_PTHREADS
// #define ENABLE_THREADS
#include "command_line_app/command_line_app.hpp"
#include "dataset/dataset.hpp"
#include "tests/tests.hpp"
#include "managers/manager.h"

#define MIN_THREAD 1

#ifdef ENABLE_THREADS

#define MAX_THREAD 1024

#ifdef ENABLE_PTHREADS
#include <pthread.h>
#include <atomic>
#include "managers/pthread_manager.hpp"

std::atomic<int> counter = {0};

#endif // ENABLE_PTHREADS

#ifdef OPENMP_VERSION
#include "managers/openmp_manager.hpp"
#endif // OPENMP_VERSION

#ifdef TBB_VERSION
#include "tbb/task_scheduler_init.h"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"
#include "tbb/cache_aligned_allocator.h"
tbb::cache_aligned_allocator<FTYPE> memory_ftype;
tbb::cache_aligned_allocator<parm> memory_parm;
#define TBB_GRAINSIZE 1
#endif // TBB_VERSION

#else

#define MAX_THREAD 1

#endif // ENABLE_THREADS

#ifdef ENABLE_PARSEC_HOOKS
#include <hooks.h>
#endif

int NUM_TRIALS = DEFAULT_NUM_TRIALS;
int nThreads = 4;
int nSwaptions = 1;
long seed = 1979; // arbitrary (but constant) default value (birth year of Christian Bienia)

long swaption_seed;
parm *swaptions;

// =================================================
FTYPE *dSumSimSwaptionPrice_global_ptr;
FTYPE *dSumSquareSimSwaptionPrice_global_ptr;
int chunksize;

void run_simulation(Range &r, parm *swaptions)
{
  FTYPE pdSwaptionPrice[2];
  for (int i = r.begin; i < r.end; i++)
  {
    int iSuccess = HJM_Swaption_Blocking(pdSwaptionPrice, swaptions[i].dStrike,
                                         swaptions[i].dCompounding, swaptions[i].dMaturity,
                                         swaptions[i].dTenor, swaptions[i].dPaymentInterval,
                                         swaptions[i].iN, swaptions[i].iFactors, swaptions[i].dYears,
                                         swaptions[i].pdYield, swaptions[i].ppdFactors,
                                         swaption_seed + i, NUM_TRIALS, BLOCK_SIZE);
    assert(iSuccess == 1);
    swaptions[i].dSimSwaptionMeanPrice = pdSwaptionPrice[0];
    swaptions[i].dSimSwaptionStdError = pdSwaptionPrice[1];
  }
}

#ifdef TBB_VERSION
struct Worker
{
  Worker() {}
  void operator()(const tbb::blocked_range<int> &range) const
  {

    int begin = range.begin();
    int end = range.end();
    Range r{range.begin(), range.end() + 1};

    run_simulation(r, swaptions);
  }
};

#endif // TBB_VERSION

void *worker(void *arg)
{
  int tid = *((int *)arg);

  Range r = find_blocked_range(tid, nSwaptions, nThreads);
  run_simulation(r, swaptions);

  return NULL;
}

// Please note: Whenever we type-cast to (int), we add 0.5 to ensure that the value is rounded to the correct number.
// For instance, if X/Y = 0.999 then (int) (X/Y) will equal 0 and not 1 (as (int) rounds down).
// Adding 0.5 ensures that this does not happen. Therefore we use (int) (X/Y + 0.5); instead of (int) (X/Y);

int main(int argc, char *argv[])
{

#ifdef PARSEC_VERSION
#define __PARSEC_STRING(x) #x
#define __PARSEC_XSTRING(x) __PARSEC_STRING(x)
  printf("PARSEC Benchmark Suite Version "__PARSEC_XSTRING(PARSEC_VERSION) "\n");
  fflush(NULL);
#else
  printf("PARSEC Benchmark Suite\n");
  fflush(NULL);
#endif // PARSEC_VERSION
#ifdef ENABLE_PARSEC_HOOKS
  __parsec_bench_begin(__parsec_swaptions);
#endif

  InputCommandLine default_input;
  default_input.seed = 1979; // arbitrary (but constant) default value (birth year of Christian Bienia)
  default_input.simulations = DEFAULT_NUM_TRIALS;
  default_input.swaptions = 1;
  default_input.threads = 1;

  SwaptionsCommandLineApp swaptions_cmd(MAX_THREAD, MIN_THREAD, default_input);
  InputCommandLine input = swaptions_cmd.get_parameters(argc, argv);
  NUM_TRIALS = input.simulations;
  nThreads = input.threads;
  nSwaptions = input.swaptions;
  seed = input.seed;

  printf("Number of Simulations: %d,  Number of threads: %d Number of swaptions: %d\n", NUM_TRIALS, nThreads, nSwaptions);
  swaption_seed = (long)(2147483647L * RanUnif(&seed));

#ifdef ENABLE_THREADS

#ifdef TBB_VERSION
  tbb::task_scheduler_init init(nThreads);
  swaptions = (parm *)memory_parm.allocate(sizeof(parm) * nSwaptions, NULL);
#endif // TBB_VERSION

  swaptions = (parm *)malloc(sizeof(parm) * nSwaptions);

#endif // ENABLE_THREADS

  Dataset dataset(seed);
  dataset.generate(swaptions, nSwaptions);

  // **********Calling the Swaption Pricing Routine*****************
#ifdef ENABLE_PARSEC_HOOKS
  __parsec_roi_begin();
#endif

#ifdef ENABLE_THREADS

  Manager *manager;

#ifdef OPENMP_VERSION
  manager = new OpenmpManager(nThreads);

#endif // OPENMP_VERSION

#ifdef TBB_VERSION
  Worker w;
  tbb::parallel_for(tbb::blocked_range<int>(0, nSwaptions, TBB_GRAINSIZE), w);

#endif // TBB_VERSION

#ifdef ENABLE_PTHREADS
  manager = new PthreadManager(nThreads);
#endif // ENABLE_PTHREADS

#else// single thread
  Range r{0, nSwaptions};
  run_simulation(r, swaptions);
#endif // ENABLE_THREADS

#ifdef ENABLE_PARSEC_HOOKS
  __parsec_roi_end();
#endif

  manager->runWorker(worker);
  Tests test;
  test.run(swaptions, nSwaptions);

  for (int i = 0; i < nSwaptions; i++)
  {
    free_dvector(swaptions[i].pdYield, 0);
    free_dmatrix(swaptions[i].ppdFactors, 0, 0);
  }

#ifdef TBB_VERSION
  memory_parm.deallocate(swaptions, sizeof(parm));
#else
  free(swaptions);
#endif // TBB_VERSION

  //***********************************************************

#ifdef ENABLE_PARSEC_HOOKS
  __parsec_bench_end();
#endif

  return 0;
}
