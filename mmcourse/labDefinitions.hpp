#ifndef LABDEFINITIONS_HPP
#define LABDEFINITIONS_HPP

#include <complex>
#include "Eigen/Dense"

namespace lab
{
  using xvalue_t = std::complex< double >;
  using point_t = std::array< xvalue_t, 3 >;

  namespace detail
  {
    xvalue_t x2(const xvalue_t & x1);
    xvalue_t x3(const xvalue_t & x1);
    double fx1(double x1);
    std::pair< xvalue_t, xvalue_t > remainfx1(double x1);
  }
  void fillJacobi(const xvalue_t & x1, const xvalue_t & x2, const xvalue_t & x3, Eigen::Matrix3cd & jcb);
  void fillSubarray(point_t & lhs, const point_t & rhs, xvalue_t (*func)(const xvalue_t &));
  bool isReal(const xvalue_t & rhs, double err);
  bool isPositive(const xvalue_t & rhs);
}

#endif
