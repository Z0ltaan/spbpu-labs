extern "C"
{
#include "cmath.h"
}

#include <iomanip>
#include <iostream>
#include <cmath>
#include <vector>
#include <iterator>
#include <fstream>

namespace lab
{
  double y1(double x)
  {
    return std::pow(1.2, x);
  }

  double y(double x)
  {
    return y1(x) - x;
  }

  double qFunc(double x)
  {
    return std::sqrt((1.0 - 0.75 * x * x) / (1 - x * x));
  }

  void fillStepValues(double arr[], double size, double step)
  {
    for (int i = 0; i < size; ++i)
    {
      arr[i] = i * step;
    }
  }

  //set number of differential equations
  constexpr int eqNumber = 2;
  using diffEType = double (*)(double, double, double);
  diffEType functions[eqNumber]{};

  int F(int n, double t, double y[], double yp[])
  {
    for (int i = 0; i < n; ++i)
      yp[i] = (*functions[i])(t, y[0], y[1]);
    return 1;
  }

  double omega = 0.0, alpha = 0.0;
  double dx1(double t, double x1, double x2)
  {
    return alpha * (x1 - (x1 * x1 * x1) / 3) - omega * omega * x2;
  }

  double dx2(double t, double x1, double x2)
  {
    return x1;
  }
}

int main()
{
  using namespace lab;

  //setup work functions
  functions[0] = &dx1;
  functions[1] = &dx2;

  // later: find left and right boundary
  for (int i = 0; i < 4; ++i)
  {
    std::cout << std::setw(2) << i << std::setw(7) << lab::y1(i) << '\n';
  }

  //calculate alpha
  double eps = 0.0000001;
  int flag = 0;
  alpha = 0.2385240 * zeroin(1.0, 2.0, &lab::y, eps, &flag);
  // alpha += alpha * 0.05;
  std::cout << alpha << '\n';

  //calculate omega
  double errest = 0.0, pos = 0.0;
  int nofun = 0;
  flag = 0;
  quanc8(&lab::qFunc, 0.0, 0.5, eps, 0.0, &omega, &errest, &nofun, &pos, &flag);
  omega *= 1.975924;
  // omega -= omega * 0.05;
  std::cout << omega << '\n';

  //set duration and step
  double T = 16.0, h = 0.4;
  double const hc = h;

  //setup vector of step values
  std::vector< double > stepValues(static_cast< int >(T / h));
  fillStepValues(stepValues.data(), stepValues.size(), h);

  //setup vectors of U and dU/dt values
  std::vector< double > U(stepValues.size());
  std::vector< double > dU(stepValues.size());
  double xs[eqNumber]{2, 0}, ds[eqNumber]{};

  double rel = eps;
  int nfe = 0, maxnfe = 30;

  rkfinit(lab::eqNumber, &flag);
  for (int i = 0; i < stepValues.size(); ++i)
  {
    flag = 1;
    rkf45(&F, eqNumber, xs, ds, &stepValues[i], stepValues[i] + hc, &rel, eps, &h, &nfe, maxnfe, &flag);
    U[i] = xs[1];
    dU[i] = xs[0];
    h = hc;
  }
  rkfend();

  //create output files for U and dU values
  std::ofstream uOut("u.txt");
  std::ofstream dUOut("dU.txt");
  std::ofstream stepOut("step.txt");

  uOut << std::fixed << std::setprecision(7);
  dUOut << std::fixed << std::setprecision(7);
  for (int i = 0; i < U.size(); ++i)
  {
    uOut << U[i] << '\n';
    dUOut << dU[i] << '\n';
    stepOut << stepValues[i] << '\n';
  }
}