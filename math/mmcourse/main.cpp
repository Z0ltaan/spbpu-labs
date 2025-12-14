#include <complex>
#include <vector>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "labDefinitions.hpp"

extern "C"
{
#include "mathlib/cmath.h"
}

namespace lab
{
  double p1 = 0.0000084, p2 = 0.00066667, p3 = 0.000017778, p5 = 2.0, p6 = 10.0, p4 = 0.01;
  constexpr int bordersize = 3;

  double dx1dt(const double & x1, const double & x2)
  {
    return (p1 * x2 - x1 * x2 + x1 - x1 * x1) / p2 - p4 * x1;
  }

  double dx2dt(const double & x1, const double & x2, const double & x3)
  {
    return (-p1 * x2 - x1 * x2 + p5 * x3) / p3 + p4 * (p6 - x2);
  }

  double dx3dt(const double & x1, const double & x3)
  {
    return x1 - x3 - p4 * x3;
  }
}


void dosmth(std::ostream & outP, std::ostream & outEi, std::ostream & outCheck)
{
  using namespace lab;

  constexpr int idxSize = 46;
  Eigen::Matrix3cd jacobi;
  jacobi(0, 2) = 0;
  jacobi(2, 0) = 1;
  jacobi(2, 1) = 0;
  jacobi(1, 2) = p5 / p3;

  int flag = 0;
  int idx = 0;
  outP << std::fixed << std::setprecision(9);
  outEi << std::fixed << std::setprecision(9);

  //parameters p4 p6
  std::array< double, idxSize > p4arr{};
  p4arr[45] = 1000.0;

  std::vector< std::array< xvalue_t, bordersize > > x1s(idxSize);
  std::vector< std::array< xvalue_t, bordersize > > x2s(idxSize);
  std::vector< std::array< xvalue_t, bordersize > > x3s(idxSize);
  xvalue_t num{};

  std::array< xvalue_t, bordersize > temp{};

  // calculate stationary points
    for (double h = 0.01; h < 101; h *= 10)
    {
      for (p4 = h; p4 < 10 * h - 0.0001; p4 += h)
      {
        p4arr[idx] = p4;

        num = zeroin(0.0, 1.0, &detail::fx1, 0.000000001, &flag);
        auto other = detail::remainfx1(num.real());

        x1s.at(idx)[0] = num;
        x1s.at(idx)[1] = other.first;
        x1s.at(idx)[2] = other.second;

        fillSubarray(x2s.at(idx), x1s.at(idx), &detail::x2);
        fillSubarray(x3s.at(idx), x1s.at(idx), &detail::x3);
        for (int k = 0; k < 3; ++k)
        {
          outCheck << p4 << ' ' << dx1dt(x1s.at(idx)[k].real(), x2s.at(idx)[k].real()) << ' '
                   << dx2dt(x1s.at(idx)[k].real(), x2s.at(idx)[k].real(), x3s.at(idx)[k].real()) << ' '
                   << dx3dt(x1s.at(idx)[k].real(), x3s.at(idx)[k].real()) << '\n';
        }

        ++idx;
      }
    }

  num = zeroin(0.0, 1.0, &detail::fx1, 0.000000001, &flag);
  auto other = detail::remainfx1(num.real());
  x1s.at(idx)[0] = num;
  x1s.at(idx)[1] = other.first;
  x1s.at(idx)[2] = other.second;

  fillSubarray(x2s.at(idx), x1s.at(idx), &detail::x2);
  fillSubarray(x3s.at(idx), x1s.at(idx), &detail::x3);

  for (int k = 0; k < 3; ++k)
  {
    outCheck << dx1dt(x1s.at(idx)[k].real(), x2s.at(idx)[k].real()) << ' '
             << dx2dt(x1s.at(idx)[k].real(), x2s.at(idx)[k].real(), x3s.at(idx)[k].real()) << ' '
             << dx3dt(x1s.at(idx)[k].real(), x3s.at(idx)[k].real()) << '\n';
  }

  // get only points with Re(x) > 0 and calculate eigenvalues
  std::vector< Eigen::Vector3cd > eigenvalues;
  std::vector< std::array< xvalue_t, bordersize > > rightXs;
  std::vector< double > p4new;
  for (int i = 0; i < x1s.size(); ++i)
  {
    p4 = p4arr[i];
    for (int j = 0; j < bordersize; ++j)
    {
      if (isPositive(x1s.at(i)[j]) && isPositive(x2s.at(i)[j]) && isPositive(x3s.at(i)[j]))
      {
        p4new.push_back(p4);
        rightXs.push_back({x1s.at(i)[j], x2s.at(i)[j], x3s.at(i)[j]});
        fillJacobi(x1s.at(i)[j], x2s.at(i)[j], x3s.at(i)[j], jacobi);
        std::cout << jacobi.determinant() << '\n';
        eigenvalues.push_back((Eigen::ComplexEigenSolver< Eigen::Matrix3cd > (jacobi, false)).eigenvalues());
      }
    }
  }

  for (int i = 0; i < rightXs.size(); ++i)
  {
    outP << std::setw(14) << p4new[i]
         << " [" << std::setw(12) << rightXs.at(i)[0].real() << ", " << std::setw(12) << rightXs.at(i)[0].imag() << "] "
         << " [" << std::setw(12) << rightXs.at(i)[1].real() << ", " << std::setw(12) << rightXs.at(i)[1].imag() << "] "
         << " [" << std::setw(12) << rightXs.at(i)[2].real() << ", " << std::setw(12) << rightXs.at(i)[2].imag() << "] "
         << "\n";


    outEi << std::setw(14) << p4new[i] << ' ';
    for (const auto & val : eigenvalues[i])
    {
      outEi << "["<< std::setw(17) <<  val.real() << std::setw(17) << val.imag() << "] ";
    }
    outEi << "\n";
  }
}
int main()
{
  using namespace lab;

  std::ofstream outP("points.txt");
  std::ofstream outEi("eigen.txt");
  std::ofstream outCheck("check.txt");

  dosmth(outP, outEi, outCheck);
  p6 = 100;
  dosmth(outP, outEi, outCheck);
}