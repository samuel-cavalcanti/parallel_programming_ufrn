
// Routines to compute various security prices using HJM framework (via Simulation).

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

#include "command_line_app/command_line_app.hpp"
// #include "tests/tests.hpp"

#include "dataset/dataset.h"

#include "runners/runner.h"
#include "runners/block_range/block_range.h"
#define MIN_THREAD 1

#ifdef ENABLE_THREADS
#define MAX_THREAD 1024
#else
#define MAX_THREAD 1
#endif // ENABLE_THREADS

#ifdef ENABLE_PARSEC_HOOKS
#include <hooks.h>
#endif // ENABLE_PARSEC_HOOKS

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
  default_input.simulation_trials = DEFAULT_NUM_TRIALS;
  default_input.swaptions = 1;
  default_input.threads = 1;

  SwaptionsCommandLineApp swaptions_cmd(MAX_THREAD, MIN_THREAD, default_input);
  InputCommandLine input = swaptions_cmd.get_parameters(argc, argv);

  auto NUM_TRIALS = input.simulation_trials;
  auto nThreads = input.threads;
  auto nSwaptions = input.swaptions;
  auto seed = input.seed;

  printf("Number of Simulation trials: %d,  Number of threads: %d Number of swaptions: %d\n", NUM_TRIALS, nThreads, nSwaptions);
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

  // Tests test;
  // test.run(swaptions,nSwaptions);

  for (auto i = 0; i < nSwaptions; i++)
  {
    fprintf(stderr, "Swaption %d: [SwaptionPrice: %.10lf StdError: %.10lf] \n",
            i, swaptions[i].dSimSwaptionMeanPrice, swaptions[i].dSimSwaptionStdError);
  }

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