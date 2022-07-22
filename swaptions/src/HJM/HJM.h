
#ifndef HJM_H

#define HJM_H

#include <assert.h>
#include "HJM_type.h"

#include <cstring>

FTYPE RanUnif(long *s);
FTYPE CumNormalInv(FTYPE u);
// void icdf_SSE(const int N, FTYPE *in, FTYPE *out);
void icdf_baseline(const int N, FTYPE *in, FTYPE *out);
// int HJM_SimPath_Forward_SSE(FTYPE **ppdHJMPath, int iN, int iFactors, FTYPE dYears, FTYPE *pdForward, FTYPE *pdTotalDrift,
// 							FTYPE **ppdFactors, long *lRndSeed);
// int Discount_Factors_SSE(FTYPE *pdDiscountFactors, int iN, FTYPE dYears, FTYPE *pdRatePath);
int Discount_Factors_opt(FTYPE *pdDiscountFactors, int iN, FTYPE dYears, FTYPE *pdRatePath);

// int HJM_SimPath_Forward_Blocking_SSE(FTYPE **ppdHJMPath, int iN, int iFactors, FTYPE dYears, FTYPE *pdForward, FTYPE *pdTotalDrift,
// 									 FTYPE **ppdFactors, long *lRndSeed, int BLOCKSIZE);


int Discount_Factors_Blocking(FTYPE *pdDiscountFactors, int iN, FTYPE dYears, FTYPE *pdRatePath, int BLOCKSIZE);


/*
extern "C" FTYPE *dvector( long nl, long nh );
extern "C" FTYPE **dmatrix( long nrl, long nrh, long ncl, long nch );
extern "C" void free_dvector( FTYPE *v, long nl, long nh );
extern "C" void free_dmatrix( FTYPE **m, long nrl, long nrh, long ncl, long nch );
*/

#endif // HJM_H