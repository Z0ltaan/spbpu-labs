#include "request.hpp"

course::SimpleRequest::SimpleRequest(const data_t &rhs) : requestData_(rhs) {}

course::SimpleRequest::data_t course::SimpleRequest::getData() const noexcept { return requestData_; }

course::SimpleRequest::SimpleRequest(deviceid_t device, requestid_t request) : requestData_(device, request) {}

// bool course::SimpleRequest::isEmpty() const noexcept { return requestData_.first == EMPTY; }
// void setData
bool course::isEmpty(const SimpleRequest &rhs) { return rhs.getData().first == EMPTY; }
