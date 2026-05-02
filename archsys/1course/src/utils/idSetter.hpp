#ifndef IDSETTER_HPP
#define IDSETTER_HPP

#include <limits>
#include <stdexcept>
#include "../priority.hpp"

namespace course
{
  template < class T >
  class IdSetter
  {
  public:
    IdSetter() : count_(0) {}

    void operator()(T &rhs)
    {
      if (count_ == std::numeric_limits< course::deviceid_t >::max())
      {
        throw std::overflow_error("Id counter reached its upper bound");
      }

      rhs.setId(count_++);
    }

  private:
    course::deviceid_t count_;
  };

} // namespace course

#endif
