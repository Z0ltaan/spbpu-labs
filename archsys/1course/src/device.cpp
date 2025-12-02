#include "device.hpp"
#include "priority.hpp"
#include "request.hpp"

course::Device::Device(const generator_t &gen) :
    deviceId_(0), processedRequestsCount_(0), currentRequest_(), nextProcessingFinish_(0.0), gen_(gen)
{}

course::Device::Device(deviceid_t id, const generator_t &gen) :
    deviceId_(id), processedRequestsCount_(0), currentRequest_(), nextProcessingFinish_(0.0), gen_(gen)
{}

course::deviceid_t course::Device::id() const noexcept { return deviceId_; }

course::requestid_t course::Device::processedRequests() const noexcept { return processedRequestsCount_; }

void course::Device::setId(course::deviceid_t id) { deviceId_ = id; }

void course::Device::setRequest(const SimpleRequest &rhs, time_t start) noexcept
{
  currentRequest_ = rhs;
  if (!rhs.empty())
  {
    time_t startTime = (start == 0.0 ? nextFinishTime() : start);
    setNextProcessingFinish(startTime + getNextProcessingTime());
  }
}

void course::Device::finishProcessing()
{
  ++processedRequestsCount_;
  setRequest(SimpleRequest());
}

course::Device::time_t course::Device::nextFinishTime() const noexcept { return nextProcessingFinish_; }

const course::SimpleRequest &course::Device::currentRequest() const noexcept { return currentRequest_; }

bool course::Device::empty() const noexcept { return currentRequest_.empty(); }
