#ifndef DEVICE_HPP
#define DEVICE_HPP

#include "priority.hpp"
#include "request.hpp"

namespace course
{
  class Device
  {
  public:
    Device();
    Device(deviceid_t id);
    virtual deviceid_t getId() const noexcept;
    virtual void setId(deviceid_t id);
    virtual void setRequest(const SimpleRequest &rhs) noexcept;
    virtual void processRequest();
    virtual SimpleRequest getRequest() const noexcept;
    virtual bool isFree() const noexcept;

  private:
    deviceid_t deviceId_;
    requestid_t processedRequestsCount_;
    SimpleRequest currentRequest_;
  };
} // namespace course

#endif
