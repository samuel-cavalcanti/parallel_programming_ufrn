#ifndef HJM_TYPE_H
#define HJM_TYPE_H
//#define DEBUG

#if defined(BASELINE) && defined(ENABLE_SSE4)
#error BASELINE and ENABLE_SSE4 are mutually exclusive
#endif

#define FTYPE double
#define BLOCK_SIZE 16 // Blocking to allow better caching


#define DEFAULT_NUM_TRIALS 102400

// arbitrary (but constant) default value (birth year of Christian Bienia)
#define DEFAULT_SEED 1979 

typedef struct
{
  int Id;
  FTYPE dSimSwaptionMeanPrice;
  FTYPE dSimSwaptionStdError;
  FTYPE dStrike;
  FTYPE dCompounding;
  FTYPE dMaturity;
  FTYPE dTenor;
  FTYPE dPaymentInterval;
  int iN;
  FTYPE dYears;
  int iFactors;
  FTYPE *pdYield;
  FTYPE **ppdFactors;
} parm;

#endif // HJM_TYPE_H
