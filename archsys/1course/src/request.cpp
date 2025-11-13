#include "request.hpp"

course::SimpleRequest::SimpleRequest(const data_t &rhs) : requestData_(rhs) {}

course::SimpleRequest::data_t course::SimpleRequest::getData() const noexcept { return requestData_; }

course::SimpleRequest::SimpleRequest(deviceid_t device, requestid_t request) : requestData_(device, request) {}

bool course::isEmpty(const SimpleRequest &rhs) { return rhs.getData().first == EMPTY; }

course::deviceid_t course::SimpleRequest::getProducerId() const noexcept { return requestData_.first; }

course::requestid_t course::SimpleRequest::getRequestNumber() const noexcept { return requestData_.second; }

bool course::operator>(const SimpleRequest &lhs, const SimpleRequest &rhs)
{
  return lhs.getProducerId() > rhs.getProducerId();
}

bool course::operator==(const SimpleRequest &lhs, const SimpleRequest &rhs)
{
  return lhs.getProducerId() == rhs.getProducerId() && lhs.getRequestNumber() == rhs.getRequestNumber();
}
