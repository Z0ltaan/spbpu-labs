#ifndef ABSTRACTBUFFER_HPP
#define ABSTRACTBUFFER_HPP

#include "abstractRequest.hpp"

namespace course
{
  class Buffer
  {
  public:
    virtual void put(const Request &request) = 0;
    virtual void pop() = 0;
    virtual Request get() = 0;
    virtual ~Buffer() = default;
  };
} // namespace course

#endif
