
#ifdef TBB_VERSION
#include <functional>
#include "tbb/task_scheduler_init.h"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"

struct Worker
{
  Worker(std::function<void(BlockRange &, parm *)> runSimulation)
  {
    this->runSimulation = runSimulation;
  } //:
  std::function<void(BlockRange &, parm *)> runSimulation;

  void operator()(const tbb::blocked_range<int> &range) const
  {

    int begin = range.begin();
    int end = range.end();
    Range r{range.begin(), range.end() + 1};

    this->run_simulation(r, swaptions);
  }
};

void runWorker(std::function<void(BlockRange &, parm *)> runSimulation, parm *input, int input_size, int nThreads)
{
  Worker w;
  tbb::parallel_for(tbb::blocked_range<int>(0, nSwaptions, TBB_GRAINSIZE), w);
}

#endif // TBB_VERSION