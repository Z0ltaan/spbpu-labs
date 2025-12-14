extern "C"
{
  #include "cmath.h"
}

#include <iostream>
#include <iomanip>
#include <cmath>
#include <iterator>
#include <algorithm>
#include <fstream>

namespace lab
{
  namespace detail
  {
    double dx1dt(double t, double x1, double x2)
    {
      return (-14) * x1 + 13 * x2 + std::cos(1 + t);
    }
    double dx2dt(double t, double x1, double x2)
    {
      return 20 * x1 - 30 * x2 + std::atan(1 + t * t);
    }
  }

  //array with diff equations
  constexpr int equationNumber = 2;
  using diffEType = double(*)(double,double,double);
  diffEType functions[equationNumber]{};

  int F(int n, double t, double y[], double yp[])
  {
    for (int i = 0; i < n; ++i)
      yp[i] = (*functions[i])(t, y[0], y[1]);
    return 1;
  }

  void rkf3(double h, double t, double x[], int order)
  {
    double k1 = 0;
    double k2 = 0;
    double k3 = 0;

    //calculate Xn+1 value
    for (int i = 0; i < order; ++i)
    {
      k1 = h * ((*functions[i])(t, x[0], x[1]));
      k2 = h * ((*functions[i])(t + h / 2, x[0] + k1 / 2, x[1] + k1 / 2));
      k3 = h * ((*functions[i])(t + 0.75 * h, x[0] + 0.75 * k2, x[1] + 0.75 * k2));
      x[i] += (2 * k1 + 3 * k2 + 4 * k3) / 9;
    }
  }

  // calculate 2 non-complex eigenvalues of 2x2 Mtx
  void calculateEigenvalues(double result[], double mtx[])
  {
    double b = -mtx[0] - mtx[3];
    double c = mtx[0] * mtx[3] - mtx[1] * mtx[2];

    double d = b * b - 4 * c;
    if (d == 0.0)
    {
      result[0] = -(b / 2);
      result[1] = result[0];
    }
    else if (d > 0.0)
    {
      result[0] = (-b + std::sqrt(d)) / 2;
      result[1] = (-b - std::sqrt(d)) / 2;
    }
    else
    {
      throw std::runtime_error("complex");
    }
  }
}


int main()
{
  int status = 0;
  lab::functions[0] = &lab::detail::dx1dt;
  lab::functions[1] = &lab::detail::dx2dt;

  //xs - array with values of x1 and x2; ds - array with values of diff
  double xs[lab::equationNumber]{2, 0.5}, ds[lab::equationNumber]{};
  double t = 0, tout = 0.075, h = 0.075, relerr = 0.0001, abserr = relerr, uBorder = 1.5;
  const double hc = h; //just a constant step value nothing more
  int nfe = 0, maxnfe = 30, flag = 1;

  //calculate solutions in t with rkf45
  std::ofstream rkfValues("rkf45.txt");

  rkfinit(lab::equationNumber, &status);
  for (int i = 0; i < static_cast< int >(uBorder / hc); ++i)
  {
    rkfValues << std::setw(5) << t + hc << "   ";
    rkf45(&lab::F, lab::equationNumber, xs, ds, &t, tout, &relerr, abserr, &h, &nfe, maxnfe, &flag);
    std::copy_n(xs, lab::equationNumber, std::ostream_iterator< double >{rkfValues, " "});
    rkfValues << "\n";
    flag = 1;
    tout += hc;
  }
  rkfend();

  std::ofstream rkf375Values("rkf3075.txt");
  xs[0] = 2;
  xs[1] = 0.5;
  t = 0;

  for (int i = 0; i < static_cast< int >(uBorder / hc); ++i)
  {
    lab::rkf3(hc, t, xs, lab::equationNumber);
    rkf375Values << std::setw(5) << t + hc << "   ";
    std::copy_n(xs, lab::equationNumber, std::ostream_iterator< double >{rkf375Values, " "});
    rkf375Values << '\n';
    t += hc;
  }

  // find custom step value and use it in rkf3
   std::ofstream rkf305Values("rkf3025.txt");
   xs[0] = 2;
   xs[1] = 0.5;
   t = 0;


  double mtx[4]{-14, 13, 20, -30};
  double eigenvalues[2]{};
  lab::calculateEigenvalues(eigenvalues, mtx);
  std::cout << 2.513 / std::max(std::abs(eigenvalues[0]), std::abs(eigenvalues[1])) << '\n';

  h = 0.5;
  for (int i = 0; i < static_cast< int >(uBorder / h); ++i)
  {
    rkf305Values << std::setw(5) << t + h << "   ";
    lab::rkf3(h, t, xs, lab::equationNumber);
    std::copy_n(xs, lab::equationNumber, std::ostream_iterator< double >{rkf305Values, " "});
    rkf305Values << '\n';
    t += h;
  }
}

