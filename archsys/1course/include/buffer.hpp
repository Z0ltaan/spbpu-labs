#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <vector>
#include "request.hpp"

namespace course
{

  class Buffer
  {
  public:
    Buffer() : Buffer(0) {};

    Buffer(size_t size) : currentlyContained_(0), buffer_(size) {}

    ~Buffer() = default;

    template < class RequestPlacementDiscipline >
    void tryAddRequest(const SimpleRequest &request, RequestPlacementDiscipline determine)
    {
      auto index = determine(*this);
      buffer_[index] = request;
      currentlyContained_++;
    }

    template < class RequestDeletionDiscipline >
    void tryGetRequest(SimpleRequest &request, RequestDeletionDiscipline determine)
    {
      auto index = determine(*this);
      request = buffer_[index];
      buffer_[index] = SimpleRequest();
      currentlyContained_--;
    }

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

    void reserve(size_t newCap) { buffer_.reserve(newCap); }

    size_t size() const noexcept { return buffer_.size(); }

    size_t contained() const noexcept { return currentlyContained_; }

    bool isFull() const noexcept { return currentlyContained_ == size(); }

    bool empty() const noexcept { return currentlyContained_ == 0; }

  private:
    size_t currentlyContained_;
    std::vector< SimpleRequest > buffer_;
  };
} // namespace course

#endif
