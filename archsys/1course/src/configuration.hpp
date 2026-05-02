#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <istream>
#include <vector>
#include "buffer.hpp"
#include "device.hpp"
#include "producer.hpp"

namespace course
{
  struct ProgramState
  {
    std::vector< Producer > producers;
    std::vector< Device > devices;
    Buffer buffer;
    requestid_t processed_requests = 0;
    requestid_t produced_requests = 0;
    requestid_t canceled_requests = 0;
    double accumulatedRequestTime = 0.0;
    double currentTime = 0.0;
  };

  struct ProgramConfiguration
  {
    deviceid_t producerCount;
    deviceid_t deviceCount;
    size_t bufferSize;
    double simulationEnd;
    double lambda;
    UniformRandomGenerator::Bounds bounds;
  };

  std::istream &operator>>(std::istream &in, ProgramConfiguration &config);

  ProgramState predictNextProgramState(const ProgramState &state);
  std::ostream &printState(std::ostream &out, const ProgramState &state);
} // namespace course

#endif
