#include <math.h>
#include <stdio.h>
#include <memory>
void first_method (int n, double *x, double *f, double *a, int min = 0);
void second_method (int n, double *x, double *f, double *a, double *d_i);
void fill_matrix (int n, double delta_x, double *matrix, double *diff, double *rhs, double *f);
void solve_system (int n, double *matrix, double *rhs, double *d_i);
void fill_matrix (int n, double delta_x, double *matrix, double *diff, double *rhs);
double lagrange (int flag, double delta_x, double *f, int nz);
double Pf (double x, int n, double a, double b, double *coeff, int i = -1);
void fill_c2 (double delta_x, double *diff, double *f, double *c2, int n, double *d_i);
double my_max (double x, double y);
double my_min (double x, double y);
