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

#ifdef ENABLE_THREADS

#ifdef ENABLE_PTHREADS
#include <pthread.h>
#include <atomic>

std::atomic<int> counter = {0};

#endif // ENABLE_PTHREADS

#define MAX_THREAD 1024
#define MIN_THREAD 1  

#ifdef OPENMP_VERSION

#include <omp.h>

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
#endif // ENABLE_THREADS

#ifdef ENABLE_PARSEC_HOOKS
#include <hooks.h>
#endif

int NUM_TRIALS = DEFAULT_NUM_TRIALS;
int nThreads = 1;
int nSwaptions = 1;
long seed = 1979; // arbitrary (but constant) default value (birth year of Christian Bienia)

long swaption_seed;
parm *swaptions;
int iN = 11;
// FTYPE dYears = 5.5;
int iFactors = 3;

// =================================================
FTYPE *dSumSimSwaptionPrice_global_ptr;
FTYPE *dSumSquareSimSwaptionPrice_global_ptr;
int chunksize;

#ifdef TBB_VERSION
struct Worker
{
  Worker() {}
  void operator()(const tbb::blocked_range<int> &range) const
  {
    FTYPE pdSwaptionPrice[2];
    int begin = range.begin();
    int end = range.end();

    for (int i = begin; i != end; i++)
    {
      int iSuccess = HJM_Swaption_Blocking(pdSwaptionPrice, swaptions[i].dStrike,
                                           swaptions[i].dCompounding, swaptions[i].dMaturity,
                                           swaptions[i].dTenor, swaptions[i].dPaymentInterval,
                                           swaptions[i].iN, swaptions[i].iFactors, swaptions[i].dYears,
                                           swaptions[i].pdYield, swaptions[i].ppdFactors,
                                           swaption_seed + i, NUM_TRIALS, BLOCK_SIZE);
      assert(iSuccess == 1);
      swaptions[i].dSimSwaptionMeanPrice = pdSwaptionPrice[0];
      swaptions[i].dSimSwaptionStdError = pdSwaptioOPENMP_VERSIONnPrice[1];
    }
  }
};

#endif // TBB_VERSION

void *worker(void *arg)
{
  int tid = *((int *)arg);
  FTYPE pdSwaptionPrice[2];

  int beg, end, chunksize;
  if (tid < (nSwaptions % nThreads))
  {
    chunksize = nSwaptions / nThreads + 1;
    beg = tid * chunksize;
    end = (tid + 1) * chunksize;
  }
  else
  {
    chunksize = nSwaptions / nThreads;
    int offsetThread = nSwaptions % nThreads;
    int offset = offsetThread * (chunksize + 1);
    beg = offset + (tid - offsetThread) * chunksize;
    end = offset + (tid - offsetThread + 1) * chunksize;
  }

  if (tid == nThreads - 1)
    end = nSwaptions;

  for (int i = beg; i < end; i++)
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

#ifdef ENABLE_PTHREADS
  counter.fetch_add(1);
#endif
  return NULL;
}

// Please note: Whenever we type-cast to (int), we add 0.5 to ensure that the value is rounded to the correct number.
// For instance, if X/Y = 0.999 then (int) (X/Y) will equal 0 and not 1 (as (int) rounds down).
// Adding 0.5 ensures that this does not happen. Therefore we use (int) (X/Y + 0.5); instead of (int) (X/Y);

int main(int argc, char *argv[])
{
  int iSuccess = 0;
  int i, j;

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
#else
  pthread_t *threads;
  pthread_attr_t pthread_custom_attr;

  threads = (pthread_t *)malloc(nThreads * sizeof(pthread_t));
  pthread_attr_init(&pthread_custom_attr);

#endif // TBB_VERSION

  if ((nThreads < 1) || (nThreads > MAX_THREAD))
  {
    fprintf(stderr, "Number of threads must be between 1 and %d.\n", MAX_THREAD);
    exit(1);
  }

#else
  if (nThreads != 1)
  {
    fprintf(stderr, "Number of threads must be 1 (serial version)\n");
    exit(1);
  }
#endif // ENABLE_THREADS

  // FTYPE **factors = NULL;
  // initialize input dataset
  // the three rows store vol data for the three factors

  FTYPE factors[3][10] = {
      // 0         1        2        3        4        5        6        7        8        9
      {.01, .01, .01, .01, .01, .01, .01, .01, .01, .01},
      {.009048, .008187, .007408, .006703, .006065, .005488, .004966, .004493, .004066, .003679},
      {.001000, .000750, .000500, .000250, .000000, .000250, .000500, .000750, .001000, .001250},
  };

  // setting up multiple swaptions
  swaptions =
#ifdef TBB_VERSION
      (parm *)memory_parm.allocate(sizeof(parm) * nSwaptions, NULL);
#else
      (parm *)malloc(sizeof(parm) * nSwaptions);
#endif

  for (i = 0; i < nSwaptions; i++)
  {
    swaptions[i].Id = i;
    swaptions[i].iN = iN;
    swaptions[i].iFactors = iFactors;
    swaptions[i].dYears = 5.0 + ((int)(60 * RanUnif(&seed))) * 0.25; // 5 to 20 years in 3 month intervals

    swaptions[i].dStrike = 0.1 + ((int)(49 * RanUnif(&seed))) * 0.1; // strikes ranging from 0.1 to 5.0 in steps of 0.1
    swaptions[i].dCompounding = 0;
    swaptions[i].dMaturity = 1.0;
    swaptions[i].dTenor = 2.0;
    swaptions[i].dPaymentInterval = 1.0;

    swaptions[i].pdYield = dvector(0, iN - 1);
    ;
    swaptions[i].pdYield[0] = .1;
    for (j = 1; j <= swaptions[i].iN - 1; ++j)
      swaptions[i].pdYield[j] = swaptions[i].pdYield[j - 1] + .005;

    swaptions[i].ppdFactors = dmatrix(0, swaptions[i].iFactors - 1, 0, swaptions[i].iN - 2);
    for (auto k = 0; k <= swaptions[i].iFactors - 1; k++)
      for (j = 0; j <= swaptions[i].iN - 2; ++j)
        swaptions[i].ppdFactors[k][j] = factors[k][j];
  }

  // **********Calling the Swaption Pricing Routine*****************
#ifdef ENABLE_PARSEC_HOOKS
  __parsec_roi_begin();
#endif

#ifdef ENABLE_THREADS

#ifdef OPENMP_VERSION
  printf("OpenMP is enabled !!\n");
#pragma omp parallel for num_threads(nThreads)
  for (i = 0; i < nThreads; i++)
  {
    int thread_id = omp_get_thread_num();
    printf("Call worked on thread id: %i\n", thread_id);
    worker(&thread_id);
  }

#endif // OPENMP_VERSION

#ifdef TBB_VERSION
  Worker w;
  tbb::parallel_for(tbb::blocked_range<int>(0, nSwaptions, TBB_GRAINSIZE), w);

#endif // TBB_VERSION

#ifdef ENABLE_PTHREADS
  printf("Pthreads is enabled !!\n");

  int *threadIDs = new int[nThreads];

  threadIDs[0] = 0;

  for (i = 1; i < nThreads; i++)
  {
    threadIDs[i] = i;
    pthread_create(&threads[i], &pthread_custom_attr, worker, &threadIDs[i]);
  }

  worker(&threadIDs[0]);

  while (counter.load() < nThreads)
  {
  } //   // waiting..

  delete[] threadIDs;

#endif // ENABLE_PTHREADS

#else
  int threadID = 0;
  worker(&threadID);
#endif // ENABLE_THREADS

#ifdef ENABLE_PARSEC_HOOKS
  __parsec_roi_end();
#endif

  auto read_test_file = [](const char *fileName)
  {
    auto stream = std::ifstream(fileName);
    std::stringstream ss;
    ss << stream.rdbuf();
    return ss.str();
  };

  std::string test_file_content = read_test_file("test.txt");

  auto string_swaptions = [](parm *swaptions)
  {
    const char template_line[] = "Swaption %d: [SwaptionPrice: %.10lf StdError: %.10lf] \n";
    std::stringstream string_stream;
    /*
      template_line é um array com 56 elementos,
      onde SwaptionPrice e StdError tem 10 digitos cada.
    */
    char line[56 + 10 + 10];

    for (auto i = 0; i < nSwaptions; i++)
    {

      sprintf(line, template_line,
              i, swaptions[i].dSimSwaptionMeanPrice, swaptions[i].dSimSwaptionStdError);

      string_stream << line;
    }

    return string_stream.str();
  };

  std::string simulation_output = string_swaptions(swaptions);

  std::cout << simulation_output;
  if (test_file_content == simulation_output)
  {
    std::cout << "passed" << std::endl;
  }
  else
  {
    std::cout << "error" << std::endl;
  }

  for (i = 0; i < nSwaptions; i++)
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

  return iSuccess;
}
