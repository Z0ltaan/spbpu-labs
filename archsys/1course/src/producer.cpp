#include "producer.hpp"
#include "request.hpp"

course::Producer::Producer() : producerId_(0), requestCount_(0) {}
course::Producer::Producer(deviceid_t id) : producerId_(id), requestCount_(0) {}

course::SimpleRequest course::Producer::produceRequest() { return SimpleRequest(getId(), requestCount_++); }

void course::Producer::setId(deviceid_t rhs) { producerId_ = rhs; }
course::deviceid_t course::Producer::getId() const { return producerId_; }

course::requestid_t course::Producer::getRequestCount() const { return requestCount_; }
