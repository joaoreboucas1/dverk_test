#pragma once

/* Fortran dverk signature (gfortran) */
typedef void (*func_dverk)(int *, double *, double *, double *);
extern void dverk_(int *n, func_dverk fcn,
                   double *x, double *y, double *xend, double *tol,
                   int *ind, double *c, int *nw, double *w);
#define dverk dverk_