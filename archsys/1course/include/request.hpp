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

    SimpleRequest() : requestData_(EMPTY, 0) {};
    // ~SimpleRequest() = default;

    SimpleRequest(const data_t &rhs);
    SimpleRequest(deviceid_t, requestid_t);

    virtual data_t getData() const noexcept;
    virtual deviceid_t getProducerId() const noexcept;
    virtual requestid_t getRequestNumber() const noexcept;

  private:
    data_t requestData_;
  };

  bool isEmpty(const SimpleRequest &rhs);
  bool operator>(const SimpleRequest &lhs, const SimpleRequest &rhs);
  bool operator==(const SimpleRequest &lhs, const SimpleRequest &rhs);
} // namespace course

#endif
