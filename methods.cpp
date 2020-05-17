#include <methods.h>

void first_method (int n, double *x, double *f, double *a, int min)
{
    double delta_x = x[1] - x[0];
    int i = 0, j = 0;
    for (i = 0; i <= n; i++)
      {
        a[i] = f[i];
      }
    for (i = n; i >= min; i--)
      {
        for (j = n; j >= n - i + 1; j--)
          {
            a[j] = (a[j] - a[j - 1]) / (delta_x * (n - i + 1));
          }
      }
}
void second_method (int n, double *x, double *f, double *diff, double *c2)
{
    double delta_x = x[1] - x[0];
    std::unique_ptr<double[]> matrix;
    std::unique_ptr<double[]>rhs;
    std::unique_ptr<double[]>d_i;

    d_i = std::make_unique<double[]>(n + 1);
    matrix = std::make_unique<double[]>(3 * n + 1);
    rhs = std::make_unique<double[]>(n + 1);

    first_method (n, x, f, diff, n);
    fill_matrix (n, delta_x, matrix.get (), diff, rhs.get (), f);
    solve_system (n, matrix.get (), rhs.get (), d_i.get ());
    fill_c2 (delta_x, diff, f, c2, n, d_i.get ());
}
void fill_matrix (int n, double delta_x, double *matrix, double *diff, double *rhs, double *f)
{
  int shift1 = n, shift2 = 2 * n + 1;
  for (int i = 0; i < n; i++)
    {
      matrix[i] = delta_x;
      matrix[i + shift1] = 4 * delta_x;
      matrix[i + shift2] = delta_x;
      rhs[i] = 3 * delta_x * diff[i] + 3 * delta_x * diff[i + 1];
    }
  matrix[n] = 1;
  rhs[0] = lagrange (0, delta_x, f, n);
  if (fabs (rhs[0]) < 1e-16)
      rhs[0] = 0;
  matrix[2 * n] = 1;
  rhs[n] = lagrange (1, delta_x, f, n);
  if (fabs (rhs[n]) < 1e-16)
      rhs[n] = 0;
  matrix[n - 1] = 0;
  matrix[shift2] = 0;
}
void
fill_c2 (double delta_x, double *diff, double *f, double *c2, int n, double *d_i)
{
    int i = 0;
    for (i = 0; i < n; i++)
    {
      c2[i] = f [i];
      c2[n + i] = d_i[i];
      c2[2 * n + i] = (3 * diff[i + 1] - 2 * d_i[i] - d_i[i + 1])
                  /(delta_x);
      c2[3 * n + i] = (d_i[i] + d_i[i + 1] - 2 * diff[i + 1])
                  / (delta_x * delta_x);
    }
}
void solve_system (int n, double *matrix, double *rhs, double *d_i)
{
  //0 -- n - 1   -- lower diagonal
  // n -- 2n     -- middle diagonal
  //2n + 1 -- 3n -- upper diagonal
  int shift1 = n, shift2 = 2 * n + 1;
  for (int i = 0; i < n; i++)
    {
      double div = matrix[i] / matrix[i + shift1];
      matrix[i + shift1 + 1] -= matrix[i + shift2] * div;
      rhs[i + 1] -= rhs[i] * div;
    }
  for (int i = n - 1; i > 0; i--)
    {
      double div = matrix[i + shift2] / matrix[i + shift1 + 1];
      rhs[i] -= rhs[i + 1] * div;
    }
  for (int i = 0; i <= n; i++)
      d_i[i] = rhs[i] / matrix[i + shift1];
}
void div (int n, double *x, double *f, double *a)
{
    double delta_x = x[1] - x[0];
    int i = 0, j = 0;
    for (i = 0; i <= n; i++)
      {
        a[i] = f[i];
      }
    for (i = n; i >= 0; i--)
      {
        for (j = n; j >= n - i + 1; j--)
          {
            a[j] = (a[j] - a[j - 1]) / (delta_x * (n - i + 1));
          }
      }
}
double lagrange (int flag, double delta_x, double *f, int n)
{
  if (n >= 4)
  {
    if (flag == 0)
      {
        double c1 = -f[0] / 6., c2 = f[1] / 2.,
               c3 = -f[2] / 2., c4 = f[3] / 6.;
        c1 *= (11);
        c2 *= (6);
        c3 *= (3);
        c4 *= (2);
        return (c1 + c2 + c3 + c4) / delta_x;
        //        return (-3. * f[0] / 2. + 2. * f[1]
//                - f[2] / 2.) / delta_x;
      }
    else
      {
        double c1 = f[n] / 6., c2 = -f[n - 1] / 2.,
               c3 = f[n - 2] / 2., c4 = -f[n - 3] / 6.;
        c1 *= 11;
        c2 *= 6;
        c3 *= 3;
        c4 *= 2;
        return (c1 + c2 + c3 + c4) / delta_x;
        return (f [n - 2] / 2. - 2. * f[n - 1]
                                        + 3. * f [n] / 2.) / delta_x;
      }
  }
  return 0;
}
double Pf (double x, int n, double a, double b, double *coeff, int i)
{
  int j = 0;
  if (i < 0)
    {
      double sum = 0, delta_x = (b - a) / n;
      for (j = n; j > 0; j--)
        {
          sum += coeff[j];
          sum *= (x - (a + delta_x * (j - 1)));
        }
      sum += coeff[0];
      return sum;
    }
  else
    {
      double delta_x = (b - a) / n;
      double c1 = coeff[i],
             c2 = coeff[n + i],
             c3 = coeff[2 * n + i],
             c4 = coeff[3 * n + i];
//      c2 *= (x - a - delta_x * i);
//      c3 *= ((x - a - delta_x * i) * (x - a - delta_x * i));
//      c4 *= ((x - a - delta_x * i) * (x - a - delta_x * i)
//             * (x - a - delta_x * i));
      return c1 + (c2 + (c3 + c4 * (x - a - delta_x * i))
                   * (x - a - delta_x * i)) * (x - a - delta_x * i);
    }
}
double my_max (double x, double y)
{
    if (x > y)
        return x;
    else
        return y;
}
double my_min (double x, double y)
{
    if (x < y)
        return x;
    else
        return y;
}
