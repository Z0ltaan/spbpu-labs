#include "labDefinitions.hpp"
namespace lab
{
  extern double p1, p2, p3, p4, p5, p6;
  namespace detail
  {
    //from dx1/dt
    xvalue_t x2(const xvalue_t & x1)
    {
      xvalue_t temp = (x1 * x1 - x1 + p4 * p2 * x1) / (p1 - x1);
      if (isReal(temp, 0.000000001))
      {
        temp = temp.real();
      }
      return temp;
    }

    //from dx3/dt
    xvalue_t x3(const xvalue_t & x1)
    {
      xvalue_t temp = x1 / (1.0 + p4);
      if (isReal(temp, 0.000000001))
      {
        temp = temp.real();
      }
      return temp;
    }

    double a = -1.0, b = 0;
    double c = 0;
    double d = 0;

    double fx1(double x1)
    {
      double temp = p5 / (1.0 + p4);
      b = (1.0 - p1 - temp - p4 * (p2 + p3));
      c = (p1 * (1.0 + temp) + p4 * (p3 * (1.0 - p6 - p2 * p4) - p2 * p1));
      d = p1 * p3 * p4 * p6;
      return a * x1 * x1 * x1 + x1 * x1 * b + x1 * c + d;
    }

    std::pair< xvalue_t, xvalue_t > remainfx1(double x1)
    {
      double f = b + x1 * a;
      double g = c + x1 * f;

      double d = f * f - 4 * a * g;
      xvalue_t temp;
      if (d == 0.0)
      {
        temp = -(b / (2 * a));
        return {temp, temp};
      }
      else if (d > 0.0)
      {
        xvalue_t t1((-f - std::sqrt(d)) / (2 * a), 0.0);
        xvalue_t t2((-f + std::sqrt(d)) / (2 * a), 0.0);
        return {t1, t2};
      }

      return {xvalue_t(-f / (2 * a), -std::sqrt(std::abs(d)) / (2 * a)),
                  xvalue_t(-f / (2 * a), std::sqrt(std::abs(d)) / (2 * a))};
    }
  }
  constexpr int bordersize = 3;

  void fillJacobi(const xvalue_t & x1, const xvalue_t & x2,
  const xvalue_t & x3, Eigen::Matrix3cd & jcb)
  {
    jcb(0, 0) = (1.0 - x2 - x1 * 2.0) / p2 - p4;
    jcb(0, 1) = (1.0 - x2 - 2.0 * x1) / p2;
    jcb(1, 0) = (-x2) / p3;
    jcb(1, 1) = (-p1 - x1) / p3 - p4;
    jcb(2, 2) = -1.0 - p4;

    // std::cout << jcb(0, 0) << ' ' << jcb(0, 1) << ' ' << jcb(1, 0) << ' ' << jcb(1, 1) << ' ' << jcb(2, 2) << '\n';
  }

  void fillSubarray(point_t & lhs, const point_t & rhs, xvalue_t (*func)(const xvalue_t &))
  {
    for (int i = 0; i < lhs.size(); ++i)
    {
      lhs[i] = (*func)(rhs[i]);
    }
  }

  bool isReal(const xvalue_t & rhs, double err)
  {
    return std::abs(rhs.imag()) < err;
  }

  bool isPositive(const xvalue_t & rhs)
  {
    return rhs.real() > 0.0;
  }
}