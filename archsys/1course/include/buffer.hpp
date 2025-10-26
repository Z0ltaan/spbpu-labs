#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <cstddef>
#include <vector>
#include "request.hpp"

namespace course
{
  class Buffer
  {
  public:
    Buffer(size_t size);
    virtual void put(const SimpleRequest &request);
    virtual SimpleRequest pop();

    const SimpleRequest &operator[](size_t index) const noexcept;
    // SimpleRequest &operator[](size_t index) noexcept;
    const SimpleRequest &at(size_t index) const;
    // SimpleRequest &at(size_t index);
    // virtual const SimpleRequest &get() const;
    size_t size() const noexcept;
    bool isFull() const noexcept;
    bool isEmpty() const noexcept;

  private:
    std::vector< SimpleRequest > buffer_;
    // dicipline
  };
} // namespace course

#endif
