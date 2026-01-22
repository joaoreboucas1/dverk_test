#include <stdio.h>
#include <math.h>
#include <gsl/gsl_odeiv2.h>
#include <gsl/gsl_errno.h>

#include "dverk.h"

void dydx_dverk(int *n, double *x, double *y, double *yprime) {
    (void) n;
    (void) x;
    *yprime = - *y;
}

int dydx_gsl(double x, const double *y, double *yprime, void *params) {
    *yprime = - *y;
    (void) x;
    (void) params;
    return GSL_SUCCESS;
}

#define INTEGRATOR_DVERK 0
#define INTEGRATOR_GSL   1
typedef struct  {
    double tol;
    int n;
    int nw;   
    int ind;
    double c[24];
    double w[10*9];
} dverk_opt;

typedef struct  {
    gsl_odeiv2_driver *driver;
} gsl_opt;

void integrate_dverk(double *x, double *y, double *x_end, dverk_opt *integrator_opt) {
    // dverk(&n, dydx_dverk, &x, &y, &x_end, &tol, &ind, c, &nw, w);
    dverk_opt opt = *integrator_opt;
    dverk(&opt.n, dydx_dverk, x, y, x_end, &opt.tol, &opt.ind, opt.c, &opt.nw, opt.w);
}

void integrate_gsl(double *x, double *y, double *x_end, gsl_opt *integrator_opt) {
    gsl_odeiv2_driver_apply(integrator_opt->driver, x, *x_end, y);
}

void integrate(double *x, double *y, double *x_end, int integrator_kind, void *integrator_opt) {
    switch (integrator_kind) {
    case INTEGRATOR_DVERK:
        integrate_dverk(x, y, x_end, (dverk_opt*) integrator_opt);
        break;
    case INTEGRATOR_GSL:
        integrate_gsl(x, y, x_end, (gsl_opt*) integrator_opt);
        break;
    default:
        abort();
        break;
    }

}

int main(void) {
    int n_dim = 1;                                 // Dimension of state
    double x_ini = 0.0, x_end = 5.0, y_ini = 1.0;  // Initial state and end of integration
    double x, y;                                   // State of the system
    double y_exact, err;                           // Compare vs exact
    double tol = 1e-12;                            // Requested absolute error
    
    // DVERK options
    dverk_opt opt_d;
    opt_d.n = n_dim;
    opt_d.ind = 1;
    opt_d.nw = 10;
    opt_d.tol = tol;

    // GSL options
    gsl_opt opt_g;
    gsl_odeiv2_system sys = {
        .function = dydx_gsl,
        .jacobian = NULL,
        .dimension = n_dim,
        .params = NULL
    };
    double hstart = 0.1;
    double epsrel = 0.0;
    opt_g.driver = gsl_odeiv2_driver_alloc_y_new(&sys, gsl_odeiv2_step_rkf45, hstart, tol, epsrel);
    
    x = x_ini;
    y = y_ini;
    integrate(&x, &y, &x_end, INTEGRATOR_DVERK, &opt_d);
    
    y_exact = exp(-x);
    err = fabs(y - y_exact);
    
    printf("--- DVERK ---\n");
    printf("x         = %.15e\n", x);
    printf("y_num     = %.15e\n", y);
    printf("y_exact   = %.15e\n", y_exact);
    printf("abs error = %.15e\n", err);
    
    x = x_ini;
    y = y_ini;    
    integrate(&x, &y, &x_end, INTEGRATOR_GSL, &opt_g);
    
    y_exact = exp(-x);
    err = fabs(y - y_exact);
    
    printf("--- GSL ---\n");
    printf("x         = %.15e\n", x);
    printf("y_num     = %.15e\n", y);
    printf("y_exact   = %.15e\n", y_exact);
    printf("abs error = %.15e\n", err);

    gsl_odeiv2_driver_free(opt_g.driver);

    return 0;
}