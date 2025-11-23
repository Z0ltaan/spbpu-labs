#ifndef PRODUCER_HPP
#define PRODUCER_HPP

#include <random>
#include "priority.hpp"

namespace course
{
  class SimpleRequest;

  class UniformRandomGenerator
  {
  public:
    using value_type = double;

    struct Bounds
    {
      value_type min;
      value_type max;
    };

    explicit UniformRandomGenerator(Bounds bounds = {0, 1}) :
        gen_(std::random_device{}()), dist_(bounds.min, bounds.max)
    {}

    UniformRandomGenerator(size_t seed, Bounds bounds = {0, 1}) : gen_(seed), dist_(bounds.min, bounds.max) {}

    value_type operator()() { return dist_(gen_); }

  private:
    std::mt19937 gen_;
    std::uniform_real_distribution< value_type > dist_;
  };

  // template < class Generator >
  class Producer
  {
  public:
    using time_t = double;
    using generator_t = UniformRandomGenerator;

    explicit Producer(generator_t = generator_t());
    Producer(deviceid_t id, generator_t = generator_t());
    SimpleRequest produceRequest();
    void setId(deviceid_t rhs);
    deviceid_t id() const noexcept;
    requestid_t requestCount() const noexcept;
    time_t time() const noexcept;

  private:
    deviceid_t producerId_;
    requestid_t requestCount_;
    time_t nextProductionTime_;
    generator_t gen_;

    void setNextProductionTime(const time_t &rhs) { nextProductionTime_ += rhs; }

    time_t getNewProductionTime() { return gen_(); }
  };
} // namespace course

#endif
