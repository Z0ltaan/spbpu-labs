#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <numeric>
#include <vector>

namespace lab
{
  namespace detail
  {
    template < class T >
    struct RightModAdder
    {
      T operator()(const T & lhs, const T & rhs)
      {
        return lhs + std::abs(rhs);
      }
    };
  }

  template < class T >
  class Matrix
  {
  public:
    using matrix_t = std::vector< T >;

    Matrix():
      mtx_(),
      rows_(0),
      clmns_(0)
    {}
    Matrix(const Matrix< T > & rhs) = default;
    Matrix(Matrix< T > && rhs) = default;
    Matrix(T * values, size_t row, size_t clmn):
      mtx_(values, values + (row * clmn)),
      rows_(row),
      clmns_(clmn)
    {}
    Matrix(size_t row, size_t clmn):
      mtx_(row * clmn),
      rows_(row),
      clmns_(clmn)
    {}

    Matrix< T > & operator=(const Matrix< T > & rhs) = default;
    Matrix< T > & operator=(Matrix< T > && rhs) = default;
    Matrix< T > operator*(const Matrix< T > & rhs) const
    {
      if (clmns_ != rhs.rows_)
        throw std::logic_error("wrong size of matrixes");
      T sum = 0;
      Matrix temp(rows_, rhs.clmns_);
      for (size_t i = 0; i < rows_; ++i)
      {
        for (size_t j = 0; j < rhs.clmns_; ++j)
        {
          sum = 0;
          for (size_t k = 0; k < rhs.rows_; ++k)
          {
            sum += get(i, k) * rhs.get(k, j);
          }
          temp.set(i, j, sum);
        }
      }
      return temp;
    }
    Matrix< T > operator-(const Matrix< T > & rhs)
    {
      if (columns() != rhs.columns() || rows() != rhs.rows())
        throw std::logic_error("wrong dimensions in -");
      Matrix< T > temp(rows(), columns());
      for (size_t i = 0; i < rows(); ++i)
      {
        for (size_t j = 0; j < columns(); ++j)
        {
          temp.set(i, j, get(i, j) - rhs.get(i, j));
        }
      }
      return temp;
    }

    const matrix_t & transpose() noexcept
    {
      matrix_t temp(mtx_.size());
      for (size_t i = 0; i < rows(); ++i)
      {
        for (size_t j = 0; j < columns(); ++j)
        {
          temp[i + j * rows()] = get(i,j);
        }
      }
      mtx_ = std::move(temp);
      std::swap(rows_, clmns_);
      return mtx_;
    }
    T calculateNorm() const noexcept
    {
      T sum = 0;
      for (size_t i = 0; i < rows(); ++i)
      {
        auto ptr = mtx_.cbegin() + i * columns();
        sum = std::max(sum, std::accumulate(ptr, ptr + columns(), T{0.0}, detail::RightModAdder< T >()));
      }
      return sum;
    }
    T * getMatrix() noexcept
    {
      return mtx_.data();
    }
    T get(size_t row, size_t clmn) const noexcept
    {
      return mtx_[clmn + row * columns()];
    }
    void set(size_t row, size_t clmn, T el) noexcept
    {
      mtx_[clmn + row * columns()] = el;
    }
    size_t rows() const noexcept
    {
      return rows_;
    }
    size_t columns() const noexcept
    {
      return clmns_;
    }

  private:
    matrix_t mtx_;
    size_t rows_;
    size_t clmns_;
  };
}

#endif