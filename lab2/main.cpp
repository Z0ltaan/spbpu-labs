extern "C"
{
  #include "cmath.h"
}

#include <iomanip>
#include <fstream>
#include <iostream>
#include <iterator>
#include "matrix.hpp"


namespace lab
{
  void fillInDValues(double ds[], int n)
  {
    double sum = 0.0;
    for (int i = 1; i <= n; ++i)
    {
      sum = 0.0;
      for (int k = 1; k <= n; ++k)
      {
        sum += 1.0 / (i + k - 1.0);
      }
      ds[i - 1] = sum;
    }
  }

  void fillInCValues(double cs[], int n)
  {
    for (int i = 0; i < n; ++i)
    {
      for (int j = 0; j < n; ++j)
      {
        cs[j + i * n] = 1.0 / (i + j + 1.0);
      }
    }
  }

  constexpr int max_n = 12;

  //matrix must be square
  Matrix< double > getInverseMatrix(double mtx[], size_t rows, size_t clmns)
  {
    Matrix< double > temp(rows, clmns);
    double cond = 0;
    int flag = 0, pivot[max_n]{};

    for (int i = 0; i < clmns; ++i)
    {
      Matrix< double > c(mtx, rows, clmns), b(rows, 1);
      b.set(i, 0, 1);

      decomp(rows, clmns, c.getMatrix(), &cond, pivot, &flag);
      solve(rows, clmns, c.getMatrix(), b.getMatrix(), pivot);

      for (int j = 0; j < rows; ++j)
        temp.set(j, i, b.get(j, 0));
    }
    return temp;
  }
}


int main()
{
  using namespace lab;
  double ds[max_n]{};
  double cs[max_n * max_n]{};

  std::ofstream deltaValues("delta.txt");
  std::ofstream condValues("cond.txt");
  std::ofstream condValues2("cond2.txt");

//  std::cout << deltaValues.is_open() << " " << condValues.is_open() << " " << condValues2.is_open() << "\n";
  double cond = 0.0;
  int pivot[max_n]{};
  int flag = 0;

  std::cout << std::setprecision(8);
  try
  {
    for (int i = 4; i <= max_n; i += 2)
    {
      //get basic values in ds and cs
      lab::fillInDValues(ds, i);
      lab::fillInCValues(cs, i);

      //create c matrix and vector of solutions (initially with d values)
      lab::Matrix< double > c(cs, i, i);
      lab::Matrix< double > x1(ds, i, 1);

      //get first vector of solutions and write down cond
      decomp(i, i, c.getMatrix(), &cond, pivot, &flag);
      condValues << cond << ' '
                 << Matrix< double >(cs, i, i).calculateNorm() * getInverseMatrix(cs, i, i).calculateNorm() << '\n';
      solve(i, i, c.getMatrix(), x1.getMatrix(), pivot);

      //T means transposed
      lab::Matrix< double > cT(cs, i, i);
      lab::Matrix< double > x2(ds, i, 1);

      c = cT;
      cT.transpose();
      c = cT * c;
      x2 = cT * x2;

      condValues2 << c.calculateNorm() * getInverseMatrix(c.getMatrix(), i, i).calculateNorm() << ' ';
      decomp(i, i, c.getMatrix(), &cond, pivot, &flag);
      condValues2 << cond << "\n";
      solve(i, i, c.getMatrix(), x2.getMatrix(), pivot);
      deltaValues << (x1 - x2).calculateNorm() / x1.calculateNorm() << '\n';
    }
  }
  catch (const std::exception & e)
  {
    std::cerr << e.what() << '\n';
  }
}