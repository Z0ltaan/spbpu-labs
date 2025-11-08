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
    virtual void processRequest(const SimpleRequest &request);
    virtual SimpleRequest::data_t getRequestData();

  private:
    deviceid_t deviceId_;
    requestid_t processedRequestsCount_;
    SimpleRequest currentRequest_;
  };
} // namespace course

#endif
