#ifndef Z_VALUES_H
#define Z_VALUES_H

#include "HJM/HJM_type.h"
/*
    Suppose that pdZ is an array of values of normal distribution, F(z)= P(Z <= z)
    find_z_values go to find the z value of F(z)    
*/
void find_z_values(FTYPE **pdZ, FTYPE **randZ, int BLOCKSIZE, int iN, int iFactors);

#endif