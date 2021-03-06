//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: Position_From_Angles_Slew.h
//
// MATLAB Coder version            : 3.2
// C/C++ source code generated on  : 21-Mar-2017 13:20:47
//
#ifndef POSITION_FROM_ANGLES_SLEW_H
#define POSITION_FROM_ANGLES_SLEW_H

// Include Files
#include <cmath>
#include <float.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rtwtypes.h"
#include "Kalman_Filter_Iteration_types.h"

// Function Declarations
extern void Position_From_Angles_Slew(const double r_E_M[3], const double q_E[4],
  const double q_M[4], double alpha_M, double beta_M, double alpha_E, double
  beta_E, const double vel[3], double time, double r_E_SC1[3], double r_E_SC2[3]);

#endif

//
// File trailer for Position_From_Angles_Slew.h
//
// [EOF]
//