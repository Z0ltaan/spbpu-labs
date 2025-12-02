#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <utility>
#include "priority.hpp"

namespace course
{
  class SimpleRequest
  {
  public:
    using data_t = std::pair< deviceid_t, requestid_t >;
    using time_t = double;

    SimpleRequest() : requestData_(EMPTY, 0), produceTime_(0.0) {};

    SimpleRequest(const data_t &rhs, time_t produceTime = 0.0);
    SimpleRequest(deviceid_t, requestid_t, time_t produceTime = 0.0);

    const data_t &data() const noexcept;
    deviceid_t producerId() const noexcept;
    requestid_t requestNumber() const noexcept;
    bool empty() const noexcept;
    time_t time() const noexcept;

  private:
    data_t requestData_;
    time_t produceTime_;
  };

  bool operator>(const SimpleRequest &lhs, const SimpleRequest &rhs);
  bool operator==(const SimpleRequest &lhs, const SimpleRequest &rhs);
} // namespace course

#endif
