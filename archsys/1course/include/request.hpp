#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <utility>
#include "priority.hpp"
// #include "producer.hpp"

namespace course
{

  class SimpleRequest //: public Request
  {
  public:
    using data_t = std::pair< deviceid_t, requestid_t >;

    SimpleRequest() = delete;
    // SimpleRequest(const SimpleRequest &rhs) = default;
    // SimpleRequest(SimpleRequest &&rhs);
    SimpleRequest(const data_t &rhs);
    SimpleRequest(deviceid_t, requestid_t);
    //
    // SimpleRequest &operator=(const SimpleRequest &rhs);
    // const SimpleRequest &operator=(const SimpleRequest &rhs) const;

    virtual data_t getData() const noexcept;
    virtual bool isEmpty() const noexcept;
    // virtual void setData(const data_t &rhs);
    // virtual void emplaceData(deviceid_t, requestid_t);
    ~SimpleRequest(); // override;

  private:
    data_t requestData_;
  };
} // namespace course

#endif
