#ifndef __HJM_SECURITIES__
#define __HJM_SECURITIES__

#include "HJM/HJM.h"

int HJM_SimPath_Yield(FTYPE **ppdHJMPath, int iN, int iFactors, FTYPE dYears, FTYPE *pdYield, FTYPE **ppdFactors,
					  long *lRndSeed);
int HJM_SimPath_Forward(FTYPE **ppdHJMPath, int iN, int iFactors, FTYPE dYears, FTYPE *pdForward, FTYPE *pdTotalDrift,
						FTYPE **ppdFactors, long *lRndSeed);
int HJM_Yield_to_Forward(FTYPE *pdForward, int iN, FTYPE *pdYield);
int HJM_Factors(FTYPE **ppdFactors, int iN, int iFactors, FTYPE *pdVol, FTYPE **ppdFacBreak);
int HJM_Drifts(FTYPE *pdTotalDrift, FTYPE **ppdDrifts, int iN, int iFactors, FTYPE dYears, FTYPE **ppdFactors);
int HJM_Correlations(FTYPE **ppdHJMCorr, int iN, int iFactors, FTYPE **ppdFactors);
int HJM_Forward_to_Yield(FTYPE *pdYield, int iN, FTYPE *pdForward);
int Discount_Factors(FTYPE *pdDiscountFactors, int iN, FTYPE dYears, FTYPE *pdRatePath);
int Discount_Factors_early_exit(FTYPE *pdDiscountFactors, int iN, FTYPE dYears, FTYPE *pdRatePath, int iSwapStartTimeIndex);
FTYPE dMax(FTYPE dA, FTYPE dB);
int HJM_Swaption_Blocking(FTYPE *pdSwaptionPrice, // Output vector that will store simulation results in the form:
												  // Swaption Price
												  // Swaption Standard Error
						  // Swaption Parameters
						  FTYPE dStrike,
						  FTYPE dCompounding, // Compounding convention used for quoting the strike (0 => continuous,
						  // 0.5 => semi-annual, 1 => annual).
						  FTYPE dMaturity,		  // Maturity of the swaption (time to expiration)
						  FTYPE dTenor,			  // Tenor of the swap
						  FTYPE dPaymentInterval, // frequency of swap payments e.g. dPaymentInterval = 0.5 implies a swap payment every half
												  // year
						  // HJM Framework Parameters (please refer HJM.cpp for explanation of variables and functions)
						  int iN,
						  int iFactors,
						  FTYPE dYears,
						  FTYPE *pdYield,
						  FTYPE **ppdFactors,
						  // Simulation Parameters
						  long iRndSeed,
						  long lTrials, int blocksize);

int HJM_SimPath_Forward_Blocking(FTYPE **ppdHJMPath, int iN, int iFactors, FTYPE dYears, FTYPE *pdForward, FTYPE *pdTotalDrift,
								 FTYPE **ppdFactors, long *lRndSeed, int BLOCKSIZE);
#endif //__HJM_SECURITIES__
