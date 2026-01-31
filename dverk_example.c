#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <omp.h>
#include "dverk.h"

static _Thread_local double b = 2.0;

void dydx_dverk(int *n, double *x, double *y, double *yprime) {
    (void) n;
    (void) x;
    *yprime = - b*(*y);
}

int main(void) {
    // Common options
    int n_dim = 1;                                 // Dimension of state
    double x_ini = 0.0, x_end = 5.0, y_ini = 1.0;  // Initial state and end of integration
    double x, y;                                   // State of the system
    double y_exact, err;                           // Compare vs exact
    double tol = 1e-8;                            // Requested absolute error
    
    // DVERK options
    int nw = 10;
    
    // Parameters to be integrated
    double b_array[5] = {0.5, 1.0, 1.5, 2.0, 2.5};
    
    #pragma omp parallel for schedule(static)
    for (size_t i = 0; i < 5; i++) {
        b = b_array[i];
        int ind = 1;
        double c[24] = {0};
        double w[10*9] = {0};
        
        double x = x_ini;
        double y = y_ini;
        dverk(&n_dim, dydx_dverk, &x, &y, &x_end, &tol, &ind, c, &nw, w);
        usleep(200*1000);

        y_exact = exp(-b*x);
        err = fabs(y - y_exact);
        
        // printf("--- b = %.2f ---\n", b);
        // printf("x         = %.15e\n", x);
        // printf("y_num     = %.15e\n", y);
        // printf("y_exact   = %.15e\n", y_exact);
        // printf("abs error = %.15e\n", err);
    }
    

    return 0;
}