
// Routines to compute various security prices using HJM framework (via Simulation).

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "nr_routines/nr_routines.h"
#include "command_line_app/command_line_app.h"
#include "swaptions_simulation/swaptions_simulation.h"

#ifdef ENABLE_PARSEC_HOOKS
#include <hooks.h>
#endif // ENABLE_PARSEC_HOOKS


#ifndef TESTS

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
  default_input.seed = DEFAULT_SEED;
  default_input.simulation_trials = DEFAULT_NUM_TRIALS;
  default_input.swaptions = 1;
  default_input.threads = 1;

  InputCommandLine input = get_parameters(default_input, argc, argv);

  printf("Number of Simulation trials: %d,  Number of threads: %d Number of swaptions: %d\n", input.simulation_trials, input.threads, input.swaptions);

  auto swaptions = new parm[input.swaptions]; /// (parm *)malloc(sizeof(parm) * nSwaptions);

  // **********Calling the Swaption Pricing Routine*****************
#ifdef ENABLE_PARSEC_HOOKS
  __parsec_roi_begin();
#endif

  run_swaptions_simulation(swaptions, input.swaptions, input.simulation_trials, input.threads, input.seed);

#ifdef ENABLE_PARSEC_HOOKS
  __parsec_roi_end();
#endif

  for (auto i = 0; i < input.swaptions; i++)
  {
    fprintf(stderr, "Swaption %d: [SwaptionPrice: %.10lf StdError: %.10lf] \n",
            i, swaptions[i].dSimSwaptionMeanPrice, swaptions[i].dSimSwaptionStdError);
  }

  for (int i = 0; i < input.swaptions; i++)
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

#endif //TESTS