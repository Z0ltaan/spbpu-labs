#ifndef PRODUCER_HPP
#define PRODUCER_HPP

#include "priority.hpp"

namespace course
{
  class SimpleRequest;

  class Producer
  {
  public:
    Producer();
    Producer(deviceid_t id);
    virtual SimpleRequest produceRequest();
    virtual void setId(deviceid_t rhs);
    virtual deviceid_t getId() const;
    virtual requestid_t getRequestCount() const;

  private:
    deviceid_t producerId_;
    requestid_t requestCount_;
  };
} // namespace course

#endif
