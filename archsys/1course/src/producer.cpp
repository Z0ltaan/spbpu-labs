#include "producer.hpp"
#include "request.hpp"

course::Producer::Producer(generator_t) : producerId_(0), requestCount_(0), nextProductionTime_(0), gen_()
{
  setNextProductionTime(getNewProductionTime());
}

course::Producer::Producer(deviceid_t id, generator_t gen) :
    producerId_(id), requestCount_(0), nextProductionTime_(0), gen_(gen)
{
  setNextProductionTime(getNewProductionTime());
}

course::SimpleRequest course::Producer::produceRequest()
{
  auto timeProduced = time();
  setNextProductionTime(getNewProductionTime());
  return SimpleRequest(id(), requestCount_++, timeProduced);
}

void course::Producer::setId(deviceid_t rhs) { producerId_ = rhs; }

course::deviceid_t course::Producer::id() const noexcept { return producerId_; }

course::Producer::time_t course::Producer::time() const noexcept { return nextProductionTime_; }

course::requestid_t course::Producer::requestCount() const noexcept { return requestCount_; }
