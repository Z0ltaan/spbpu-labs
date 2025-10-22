#ifndef ABSTRACTDEVICE_HPP
#define ABSTRACTDEVICE_HPP

#include "abstractRequest.hpp"

namespace course
{
  class Device
  {
  public:
    virtual void processRequest(const Request &request) = 0;
    virtual ~Device() = default;
  };
} // namespace course
#endif
