# Comparison between GSL and DVERK

DVERK is a numerical integrator written in Fortran 66 by T. Hull, W. Enright and K. Jackson from University of Toronto. The DVERK integrator is still used in the cosmological code [CAMB](https://github.com/cmbant/CAMB). This project has a few aims:
- Investigate how to integrate DVERK with C code
- Compare DVERK with GSL

The source is in `dverk.f`; `dverk.h` contains C bindings for DVERK; `main.c` contains an example using both integrators; `Makefile` builds the example.

## References:
- [Users Guide for DVERK](./Users.Guide.For.DVERK.pdf) by Hull, Enright and Jackson
- [GSL odeiv2 manual](https://www.gnu.org/software/gsl/doc/html/ode-initval.html)