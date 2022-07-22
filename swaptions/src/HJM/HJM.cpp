
#include "HJM.h"

#include "../nr_routines/nr_routines.h"
#include <math.h>

// ***********************************************************************
// ***********************************************************************
// ***********************************************************************
int Discount_Factors_Blocking(FTYPE *pdDiscountFactors,
                              int iN,
                              FTYPE dYears,
                              FTYPE *pdRatePath,
                              int BLOCKSIZE)
{
    int i, j, b;  // looping variables
    int iSuccess; // return variable

    FTYPE ddelt; // HJM time-step length
    ddelt = (FTYPE)(dYears / iN);

    FTYPE *pdexpRes;
    pdexpRes = dvector(0, (iN - 1) * BLOCKSIZE - 1);
    // precompute the exponientials
    for (j = 0; j <= (iN - 1) * BLOCKSIZE - 1; ++j)
    {
        pdexpRes[j] = -pdRatePath[j] * ddelt;
    }
    for (j = 0; j <= (iN - 1) * BLOCKSIZE - 1; ++j)
    {
        pdexpRes[j] = exp(pdexpRes[j]);
    }

    // initializing the discount factor vector
    for (i = 0; i < (iN)*BLOCKSIZE; ++i)
        pdDiscountFactors[i] = 1.0;

    for (i = 1; i <= iN - 1; ++i)
    {
        // printf("\nVisiting timestep %d : ",i);
        for (b = 0; b < BLOCKSIZE; b++)
        {
            // printf("\n");
            for (j = 0; j <= i - 1; ++j)
            {
                pdDiscountFactors[i * BLOCKSIZE + b] *= pdexpRes[j * BLOCKSIZE + b];
                // printf("(%f) ",pdexpRes[j*BLOCKSIZE + b]);
            }
        } // end Block loop
    }

    free_dvector(pdexpRes, 0);
    iSuccess = 1;
    return iSuccess;
}

FTYPE RanUnif(long *s)
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


// Returns the inverse of cumulative normal distribution function.
// Reference: Moro, B., 1995, "The Full Monte," RISK (February), 57-58.
FTYPE CumNormalInv(FTYPE u)
{
  /**********************************************************************/
  static FTYPE a[4] = {
      2.50662823884,
      -18.61500062529,
      41.39119773534,
      -25.44106049637};

  static FTYPE b[4] = {
      -8.47351093090,
      23.08336743743,
      -21.06224101826,
      3.13082909833};

  static FTYPE c[9] = {
      0.3374754822726147,
      0.9761690190917186,
      0.1607979714918209,
      0.0276438810333863,
      0.0038405729373609,
      0.0003951896511919,
      0.0000321767881768,
      0.0000002888167364,
      0.0000003960315187};

  /**********************************************************************/


  FTYPE x, r;

  x = u - 0.5;
  if (fabs(x) < 0.42)
  {
    r = x * x;
    r = x * (((a[3] * r + a[2]) * r + a[1]) * r + a[0]) /
        ((((b[3] * r + b[2]) * r + b[1]) * r + b[0]) * r + 1.0);
    return (r);
  }

  r = u;
  if (x > 0.0)
    r = 1.0 - u;
  r = log(-log(r));
  r = c[0] + r * (c[1] + r *
                             (c[2] + r * (c[3] + r *
                                                     (c[4] + r * (c[5] + r * (c[6] + r * (c[7] + r * c[8])))))));
  if (x < 0.0)
    r = -r;

  return (r);
} 


void icdf_baseline(const int N, FTYPE *in, FTYPE *out)
{

  FTYPE z, r;

  const FTYPE
      a1 = -3.969683028665376e+01,
      a2 = 2.209460984245205e+02,
      a3 = -2.759285104469687e+02,
      a4 = 1.383577518672690e+02,
      a5 = -3.066479806614716e+01,
      a6 = 2.506628277459239e+00;

  const FTYPE
      b1 = -5.447609879822406e+01,
      b2 = 1.615858368580409e+02,
      b3 = -1.556989798598866e+02,
      b4 = 6.680131188771972e+01,
      b5 = -1.328068155288572e+01;

  const FTYPE
      c1 = -7.784894002430293e-03,
      c2 = -3.223964580411365e-01,
      c3 = -2.400758277161838e+00,
      c4 = -2.549732539343734e+00,
      c5 = 4.374664141464968e+00,
      c6 = 2.938163982698783e+00;

  const FTYPE
      // d0 =  0.0,
      d1 = 7.784695709041462e-03,
      d2 = 3.224671290700398e-01,
      d3 = 2.445134137142996e+00,
      d4 = 3.754408661907416e+00;

  // Limits of the approximation region.
#define U_LOW 0.02425

  const FTYPE u_low = U_LOW, u_high = 1.0 - U_LOW;

  for (int i = 0; i < N; i++)
  {
    FTYPE u = in[i];
    // Rational approximation for the lower region. ( 0 < u < u_low )
    if (u < u_low)
    {
      z = sqrt(-2.0 * log(u));
      z = (((((c1 * z + c2) * z + c3) * z + c4) * z + c5) * z + c6) / ((((d1 * z + d2) * z + d3) * z + d4) * z + 1.0);
    }
    // Rational approximation for the central region. ( u_low <= u <= u_high )
    else if (u <= u_high)
    {
      z = u - 0.5;
      r = z * z;
      z = (((((a1 * r + a2) * r + a3) * r + a4) * r + a5) * r + a6) * z / (((((b1 * r + b2) * r + b3) * r + b4) * r + b5) * r + 1.0);
    }
    // Rational approximation for the upper region. ( u_high < u < 1 )
    else
    {
      z = sqrt(-2.0 * log(1.0 - u));
      z = -(((((c1 * z + c2) * z + c3) * z + c4) * z + c5) * z + c6) / ((((d1 * z + d2) * z + d3) * z + d4) * z + 1.0);
    }
    out[i] = z;
  }
  return;
}

int Discount_Factors_opt(FTYPE *pdDiscountFactors,
						 int iN,
						 FTYPE dYears,
						 FTYPE *pdRatePath)
{
	int i, j;	  // looping variables
	int iSuccess; // return variable

	FTYPE ddelt; // HJM time-step length
	ddelt = (FTYPE)(dYears / iN);

	FTYPE *pdexpRes;
	pdexpRes = dvector(0, iN - 2);

	// initializing the discount factor vector
	for (i = 0; i <= iN - 1; ++i)
		pdDiscountFactors[i] = 1.0;

	// precompute the exponientials
	for (j = 0; j <= (i - 2); ++j)
	{
		pdexpRes[j] = -pdRatePath[j] * ddelt;
	}
	for (j = 0; j <= (i - 2); ++j)
	{
		pdexpRes[j] = exp(pdexpRes[j]);
	}

	for (i = 1; i <= iN - 1; ++i)
		for (j = 0; j <= i - 1; ++j)
			pdDiscountFactors[i] *= pdexpRes[j];

	free_dvector(pdexpRes, 0);
	iSuccess = 1;
	return iSuccess;
}
