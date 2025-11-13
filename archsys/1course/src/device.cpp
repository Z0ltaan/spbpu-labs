#include "device.hpp"
#include <thread>
#include "priority.hpp"
#include "request.hpp"

course::Device::Device() : deviceId_(0), processedRequestsCount_(0) {}

course::Device::Device(deviceid_t id) : deviceId_(id), processedRequestsCount_(0) {}

course::deviceid_t course::Device::getId() const noexcept { return deviceId_; }

void course::Device::setId(course::deviceid_t id) { deviceId_ = id; }

void course::Device::setRequest(const SimpleRequest &rhs) noexcept { currentRequest_ = rhs; }

void course::Device::processRequest()
{
  std::this_thread::sleep_for(std::chrono::milliseconds(30));
  ++processedRequestsCount_;
}

course::SimpleRequest course::Device::getRequest() const noexcept { return currentRequest_; }

bool course::Device::isFree() const noexcept { return isEmpty(currentRequest_); }
