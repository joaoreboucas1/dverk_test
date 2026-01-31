#include <stdio.h>
#include <math.h>
#include "integrators.c"

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

int main(void) {
    // Common options
    int n_dim = 1;                                 // Dimension of state
    double x_ini = 0.0, x_end = 5.0, y_ini = 1.0;  // Initial state and end of integration
    double x, y;                                   // State of the system
    double y_exact, err;                           // Compare vs exact
    double tol = 1e-12;                            // Requested absolute error
    
    double c[DVERK_C_CAPACITY];
    double w[DVERK_W_CAPACITY];
    integrator_opt opt1 = get_dverk_integrator(dydx_dverk, tol, n_dim, 1, c, w);
    integrator_opt opt2 = get_gsl_integrator(dydx_gsl, tol, n_dim);
    
    x = x_ini;
    y = y_ini;
    integrate(&x, &y, &x_end, &opt1);
    
    y_exact = exp(-x);
    err = fabs(y - y_exact);
    
    printf("--- DVERK ---\n");
    printf("x         = %.15e\n", x);
    printf("y_num     = %.15e\n", y);
    printf("y_exact   = %.15e\n", y_exact);
    printf("abs error = %.15e\n", err);
    
    x = x_ini;
    y = y_ini;    
    integrate(&x, &y, &x_end, &opt2);
    
    y_exact = exp(-x);
    err = fabs(y - y_exact);
    
    printf("--- GSL ---\n");
    printf("x         = %.15e\n", x);
    printf("y_num     = %.15e\n", y);
    printf("y_exact   = %.15e\n", y_exact);
    printf("abs error = %.15e\n", err);

    integrator_free(opt1);
    integrator_free(opt2);

    return 0;
}