#include "producer.hpp"
#include "request.hpp"

course::Producer::Producer(const generator_t &gen) :
    producerId_(0), requestCount_(0), prevProductionTime_(0.0), nextProductionTime_(0), gen_(gen)
{
  setNextProductionTime(getNewProductionTime());
}

course::Producer::Producer(deviceid_t id, const generator_t &gen) :
    producerId_(id), requestCount_(0), prevProductionTime_(0.0), nextProductionTime_(0), gen_(gen)
{
  setNextProductionTime(getNewProductionTime());
}

course::SimpleRequest course::Producer::produceRequest()
{
  // auto prodTime = nextProductionTime();
  prevProductionTime_ = nextProductionTime();
  setNextProductionTime(getNewProductionTime());
  return SimpleRequest(id(), requestCount_++, prevProductionTime_);
}

void course::Producer::setId(deviceid_t rhs) { producerId_ = rhs; }

course::deviceid_t course::Producer::id() const noexcept { return producerId_; }

course::Producer::time_t course::Producer::prevProductionTime() const noexcept { return prevProductionTime_; }

course::Producer::time_t course::Producer::nextProductionTime() const noexcept { return nextProductionTime_; }

course::requestid_t course::Producer::requestCount() const noexcept { return requestCount_; }
