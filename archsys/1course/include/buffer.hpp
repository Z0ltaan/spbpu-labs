#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <algorithm>
#include <cstddef>
#include <vector>
#include "request.hpp"

namespace course
{
  template < class Container >
  class Buffer
  {
  public:
    Buffer(size_t size);

    const SimpleRequest &operator[](size_t index) const noexcept { return buffer_[index]; }

    SimpleRequest &operator[](size_t index) noexcept
    {
      return const_cast< SimpleRequest & >(static_cast< const Buffer & >(*this)[index]);
    }
    const SimpleRequest &at(size_t index) const { return buffer_.at(index); }
    SimpleRequest &at(size_t index)
    {
      return const_cast< SimpleRequest & >(static_cast< const Buffer & >(*this).at(index));
    }

    std::vector< SimpleRequest >::const_iterator begin() const noexcept { return buffer_.begin(); }

    std::vector< SimpleRequest >::const_iterator end() const noexcept { return buffer_.end(); }
    std::vector< SimpleRequest >::iterator begin() noexcept
    {
      return std::vector< course::SimpleRequest >::iterator(this->begin());
    }
    std::vector< SimpleRequest >::iterator end() noexcept
    {
      return std::vector< course::SimpleRequest >::iterator(this->end());
    }

    size_t size() const noexcept { return buffer_.size(); }
    bool isFull() const noexcept { return std::none_of(buffer_.begin(), buffer_.end(), isEmptySlot); }
    bool isEmpty() const noexcept { return std::all_of(buffer_.begin(), buffer_.end(), isEmptySlot); }

  private:
    Container buffer_;
    // dicipline

    bool isEmptySlot(const SimpleRequest &item) const noexcept { return item.getData().first == EMPTY; }
  };
} // namespace course

#endif
