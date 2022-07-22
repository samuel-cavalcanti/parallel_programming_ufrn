#include "swaptions_simulation.h"
#include "dataset/dataset.h"
#include "HJM_Securities/HJM_Securities.h"
#include "block_range/block_range.h"
#include "runners/runner.h"


void run_swaptions_simulation(parm *swaptions, int nSwaptions, int simulation_trials, int nThreads, long seed)
{

    auto swaption_seed = (long)(2147483647L * RanUnif(&seed));

    createRandomDataset(swaptions, nSwaptions, seed);

    auto run_simulation = [swaption_seed, simulation_trials](BlockRange &r, parm *swaptions)
    {
        FTYPE pdSwaptionPrice[2];
        for (int i = r.begin; i < r.end; i++)
        {
            int iSuccess = HJM_Swaption_Blocking(pdSwaptionPrice, swaptions[i].dStrike,
                                                 swaptions[i].dCompounding, swaptions[i].dMaturity,
                                                 swaptions[i].dTenor, swaptions[i].dPaymentInterval,
                                                 swaptions[i].iN, swaptions[i].iFactors, swaptions[i].dYears,
                                                 swaptions[i].pdYield, swaptions[i].ppdFactors,
                                                 swaption_seed + i, simulation_trials, BLOCK_SIZE);
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
}