#ifndef ABSTRACTREQUEST_HPP
#define ABSTRACTREQUEST_HPP

namespace course
{
  class Request
  {
  public:
    virtual void getData() = 0;
    virtual ~Request() = default;
  };
} // namespace course
#endif
