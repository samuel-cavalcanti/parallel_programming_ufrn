#ifndef DATASET_HPP
#define DATASET_HPP

#include "../HJM_type.h"
#include "../nr_routines.h"
class Dataset
{
private:
    FTYPE factors[3][10] = {
        // 0         1        2        3        4        5        6        7        8        9
        {.01, .01, .01, .01, .01, .01, .01, .01, .01, .01},
        {.009048, .008187, .007408, .006703, .006065, .005488, .004966, .004493, .004066, .003679},
        {.001000, .000750, .000500, .000250, .000000, .000250, .000500, .000750, .001000, .001250},
    };
    int iN = 11;
    // FTYPE dYears = 5.5;
    int iFactors = 3;
    long seed;

    FTYPE RanUnif(long *s);

public:
    Dataset(long seed);

    ~Dataset();

    void generate(parm *swaptions, int &nSwaptions);
};

Dataset::Dataset(long seed)
{
    this->seed = seed;
}

Dataset::~Dataset()
{
}

FTYPE Dataset::RanUnif(long *s)
{
    // RanUnif.c
    // Author: Mark Broadie
    // Collaborator: Mikhail Smelyanskiy, Intel

    /* See "Random Number Generators: Good Ones Are Hard To Find", */
    /*     Park & Miller, CACM 31#10 October 1988 pages 1192-1201. */
    // uniform random number generator

    long ix, k1;
    FTYPE dRes;

    ix = *s;
    k1 = ix / 127773L;
    ix = 16807L * (ix - k1 * 127773L) - k1 * 2836L;
    if (ix < 0)
        ix = ix + 2147483647L;
    *s = ix;
    dRes = (ix * 4.656612875e-10);
    return (dRes);

} // end of RanUnif

void Dataset::generate(parm *swaptions, int &nSwaptions)
{
    for (int i = 0; i < nSwaptions; i++)
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

        swaptions[i].pdYield[0] = .1;
        for (int j = 1; j <= swaptions[i].iN - 1; ++j)
            swaptions[i].pdYield[j] = swaptions[i].pdYield[j - 1] + .005;

        swaptions[i].ppdFactors = dmatrix(0, swaptions[i].iFactors - 1, 0, swaptions[i].iN - 2);
        for (auto k = 0; k <= swaptions[i].iFactors - 1; k++)
            for (int j = 0; j <= swaptions[i].iN - 2; ++j)
                swaptions[i].ppdFactors[k][j] = factors[k][j];
    }
}

#endif // DATASET_HPP