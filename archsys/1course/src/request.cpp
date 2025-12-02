#include "request.hpp"

course::SimpleRequest::SimpleRequest(const data_t &rhs, time_t produceTime) :
    requestData_(rhs), produceTime_(produceTime)
{}

const course::SimpleRequest::data_t &course::SimpleRequest::data() const noexcept { return requestData_; }

course::SimpleRequest::SimpleRequest(deviceid_t device, requestid_t request, time_t produceTime) :
    requestData_(device, request), produceTime_(produceTime)
{}

bool course::SimpleRequest::empty() const noexcept { return requestData_.first == EMPTY; }
course::deviceid_t course::SimpleRequest::producerId() const noexcept { return requestData_.first; }

course::requestid_t course::SimpleRequest::requestNumber() const noexcept { return requestData_.second; }

bool course::operator>(const SimpleRequest &lhs, const SimpleRequest &rhs)
{
  return lhs.producerId() > rhs.producerId();
}

bool course::operator==(const SimpleRequest &lhs, const SimpleRequest &rhs)
{
  return lhs.producerId() == rhs.producerId() && lhs.requestNumber() == rhs.requestNumber();
}

course::SimpleRequest::time_t course::SimpleRequest::time() const noexcept { return produceTime_; }
