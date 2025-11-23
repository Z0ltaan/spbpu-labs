#include "mainLogic.hpp"
#include <algorithm>
#include <cstddef>
#include <exception>
#include <iostream>
#include <iterator>
#include <ostream>
#include <vector>
#include "buffer.hpp"
#include "device.hpp"
#include "disciplines.hpp"
#include "priority.hpp"
#include "producer.hpp"
#include "request.hpp"
#include "utils/idSetter.hpp"

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
  } config{4, 10, 10, 100.0};

  std::ostream &printState(std::ostream &out, const ProgramState &state);
  ProgramState predictNextProgramState(const ProgramState &state);
} // namespace course

int course::mainLogic(int argc, char **argv)
{
  ProgramState current_state{std::vector< Producer >(), std::vector< Device >(), Buffer(config.bufferSize)};

  {
    UniformRandomGenerator prodgen(UniformRandomGenerator::Bounds{2.0, 12.0});
    std::generate_n(std::back_insert_iterator(current_state.producers), config.producerCount,
                    [&] { return Producer(prodgen); });

    ExponentialRandomGenerator devgen(7.0);
    std::generate_n(std::back_insert_iterator(current_state.devices), config.deviceCount,
                    [&] { return Device(devgen); });
  }

  std::for_each(current_state.producers.begin(), current_state.producers.end(), course::IdSetter< course::Producer >());
  std::for_each(current_state.devices.begin(), current_state.devices.end(), course::IdSetter< course::Device >());

  std::vector< ProgramState > programFlow;
  while (current_state.currentTime < config.simulationEnd)
  {
    programFlow.push_back(current_state);
    current_state = predictNextProgramState(current_state);
  }

  std::for_each(programFlow.begin(), programFlow.end(),
                [&](const ProgramState &item) -> void { printState(std::cout, item); });
  printState(std::cout, current_state);

  return 0;
}

namespace course
{
  struct Event
  {
    double timestamp;
    size_t position;

    enum EventType
    {
      PRODUCTION,
      COMPLETION,
    } type;
  };

  // write adequate function and put it in cycle and THEN we're done
  ProgramState predictNextProgramState(const ProgramState &state)
  {
    auto newState{state};

    // Determine closest special event
    Event closestEvent = {state.producers[0].time(), 0, Event::EventType::PRODUCTION};
    for (size_t i = 1; i < state.producers.size(); ++i)
    {
      const Producer &current = state.producers[i];
      if (current.time() < closestEvent.timestamp)
      {
        closestEvent = {current.time(), i, Event::EventType::PRODUCTION};
      }
    }

    for (size_t i = 0; i < state.devices.size(); ++i)
    {
      const Device &current = state.devices[i];
      if (!current.empty() && current.time() < closestEvent.timestamp)
      {
        closestEvent = {current.time(), i, Event::EventType::COMPLETION};
      }
    }
    newState.currentTime = closestEvent.timestamp;

    if (closestEvent.type == Event::EventType::PRODUCTION)
    {
      auto &neededProducer = newState.producers[closestEvent.position];
      SimpleRequest req = neededProducer.produceRequest();
      newState.produced_requests += 1;

      try
      {

        if (state.buffer.empty())
        {
          try
          {
            auto deviceIndex = course::chooseDeviceById(newState.devices);
            Device &chosenDevice = newState.devices[deviceIndex];
            chosenDevice.setRequest(req, req.time());
          }
          catch (const std::exception &e)
          {
            std::cerr << e.what() << '\n';
            newState.buffer.tryAddRequest(req, chooseFirstEmptyBufferSpot);
          }
        }
        else
        {
          newState.buffer.tryAddRequest(req, chooseFirstEmptyBufferSpot);
        }
      }
      catch (const std::exception &e)
      {
        std::cerr << e.what() << '\n';
        newState.canceled_requests += 1;
      }
    }
    else if (closestEvent.type == Event::EventType::COMPLETION)
    {
      Device &chosenDevice = newState.devices[closestEvent.position];
      newState.accumulatedRequestTime += (chosenDevice.time() - chosenDevice.currentRequest().time());
      chosenDevice.finishProcessing();
      newState.processed_requests += 1;

      // NOTE: setRequest here works fine only if special events are prod, fin, and simfin
      try
      {
        SimpleRequest req;
        newState.buffer.tryGetRequest(req, chooseRequestFromBufferByProducerNumber);
        chosenDevice.setRequest(req);
      }
      catch (const std::exception &e)
      {
        std::cerr << e.what() << '\n';
      }
    }
    return newState;
  }

  std::ostream &printState(std::ostream &out, const ProgramState &state)
  {
    std::string producers("P: ");
    for (const Producer &p: state.producers)
    {
      try
      {
        producers += " " + std::to_string(p.requestCount());
      }
      catch (const std::bad_alloc &e)
      {
        std::cerr << e.what() << '\n';
      }
    }
    producers += '\n';

    std::string devices("D: ");
    for (const Device &d: state.devices)
    {
      try
      {
        devices += " " + std::to_string(d.processedRequests());
      }
      catch (const std::bad_alloc &e)
      {
        std::cerr << e.what() << '\n';
      }
    }
    devices += '\n';
    out << producers << devices << "processed: " << state.processed_requests << '\n';
    out << "produced: " << state.produced_requests << '\n' << "canceled: " << state.canceled_requests << '\n';
    out << "current time: " << state.currentTime << '\n';
    return out;
  }

} // namespace course
