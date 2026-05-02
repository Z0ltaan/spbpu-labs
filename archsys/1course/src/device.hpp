#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <random>
#include "priority.hpp"
#include "request.hpp"

namespace course
{
  class ExponentialRandomGenerator
  {
  public:
    using value_type = double;

    explicit ExponentialRandomGenerator(value_type lambda = 1.0) : gen_(std::random_device{}()), dist_(1.0 / lambda) {}

    ExponentialRandomGenerator(size_t seed, value_type lambda = 1.0) : gen_(seed), dist_(1.0 / lambda) {}

    value_type operator()() { return dist_(gen_); }

  private:
    std::mt19937 gen_;
    std::exponential_distribution< value_type > dist_;
  };

  class Device
  {
  public:
    using time_t = double;
    using generator_t = ExponentialRandomGenerator;

    explicit Device(const generator_t &gen);
    Device(deviceid_t id, const generator_t &gen);

    deviceid_t id() const noexcept;
    requestid_t processedRequests() const noexcept;
    void setId(deviceid_t id);
    void setRequest(const SimpleRequest &rhs, time_t start = 0.0) noexcept;
    void finishProcessing();
    time_t nextFinishTime() const noexcept;
    const SimpleRequest &currentRequest() const noexcept;
    bool empty() const noexcept;

  private:
    deviceid_t deviceId_;
    requestid_t processedRequestsCount_;
    SimpleRequest currentRequest_;
    time_t nextProcessingFinish_;
    generator_t gen_;

    void setNextProcessingFinish(time_t rhs) { nextProcessingFinish_ = rhs; }

    time_t getNextProcessingTime() { return gen_(); }
  };
} // namespace course

#endif
