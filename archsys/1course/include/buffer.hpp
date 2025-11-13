#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include "request.hpp"

namespace course
{
  // extern std::atomic< size_t > producersLeft;
  extern volatile size_t producersLeft;

  class Buffer
  {
  public:
    Buffer(size_t size) : currentlyContained_(0), buffer_(size), mutex_(), cv_() {}

    ~Buffer() = default;

    template < class RequestPlacementDiscipline >
    void tryAddRequest(const SimpleRequest &request, RequestPlacementDiscipline determine)
    {
      std::unique_lock< std::mutex > lock(mutex_);
      std::cout << producersLeft << " tryadd\n";
      auto index = determine(*this);
      buffer_[index] = request;
      currentlyContained_++;
      // std::cout << producersLeft << " thr\n";
      // std::cout << std::this_thread::get_id() << " thr\n";

      lock.unlock();
      // cv_.notify_one();
    }

    template < class RequestDeletionDiscipline >
    void tryGetRequest(SimpleRequest &request, RequestDeletionDiscipline determine)
    {
      std::unique_lock< std::mutex > lock(mutex_);
      std::cout << "tryget\n";
      auto index = determine(*this);
      request = buffer_[index];
      buffer_[index] = SimpleRequest();
      currentlyContained_--;
    }

    void waitForNotEmpty()
    {
      std::unique_lock< std::mutex > lock(mutex_);
      std::cout << producersLeft << " wait\n";
      cv_.wait(lock, [] { return producersLeft == 0; });
      // cv_.wait(lock, [this] { return !this->isEmpty(); });
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

    size_t size() const noexcept { return buffer_.size(); }

    inline bool isFull() const noexcept
    {
      return currentlyContained_ == size();
    } //{ return std::none_of(buffer_.begin(), buffer_.end(), course::isEmpty); }

    inline bool isEmpty() const noexcept
    {
      return currentlyContained_ == 0;
    } //{ return std::all_of(buffer_.begin(), buffer_.end(), course::isEmpty); }

  private:
    volatile size_t currentlyContained_;
    std::vector< SimpleRequest > buffer_;
    std::mutex mutex_;
    std::condition_variable cv_;
  };
} // namespace course

#endif
