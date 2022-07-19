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
#include "dataset/dataset.h"

#include "runners/runner.h"
#include "runners/block_range/block_range.h"
#define MIN_THREAD 1

#ifdef ENABLE_THREADS

#define MAX_THREAD 1024

#ifdef ENABLE_PTHREADS

#endif // ENABLE_PTHREADS

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


  auto NUM_TRIALS = input.simulations;
  auto nThreads = input.threads;
  auto nSwaptions = input.swaptions;
  auto seed = input.seed;

  printf("Number of Simulations: %d,  Number of threads: %d Number of swaptions: %d\n", NUM_TRIALS, nThreads, nSwaptions);
  auto swaption_seed = (long)(2147483647L * RanUnif(&seed));

  auto swaptions = new parm[nSwaptions]; /// (parm *)malloc(sizeof(parm) * nSwaptions);

  createRandomDataset(swaptions, nSwaptions, seed);

  // **********Calling the Swaption Pricing Routine*****************
#ifdef ENABLE_PARSEC_HOOKS
  __parsec_roi_begin();
#endif

  auto run_simulation = [swaption_seed, NUM_TRIALS](BlockRange &r, parm *swaptions)
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
  };

#ifdef ENABLE_THREADS
  runWorker(run_simulation, swaptions, nSwaptions, nThreads);
#else  // single thread
  BlockRange r{0, nSwaptions};
  run_simulation(r, swaptions);
#endif // ENABLE_THREADS

#ifdef ENABLE_PARSEC_HOOKS
  __parsec_roi_end();
#endif

   for (auto i = 0; i < nSwaptions; i++)
  {
    fprintf(stderr, "Swaption %d: [SwaptionPrice: %.10lf StdError: %.10lf] \n",
            i, swaptions[i].dSimSwaptionMeanPrice, swaptions[i].dSimSwaptionStdError);
  }
  // Tests test;
  // test.run(swaptions, nSwaptions);

  for (int i = 0; i < nSwaptions; i++)
  {
    free_dvector(swaptions[i].pdYield, 0);
    free_dmatrix(swaptions[i].ppdFactors, 0, 0);
  }

  delete[] swaptions;

  //***********************************************************

#ifdef ENABLE_PARSEC_HOOKS
  __parsec_bench_end();
#endif

  return 0;
}
