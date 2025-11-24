#include "configuration.hpp"
#include <iostream>
#include "disciplines.hpp"

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

course::ProgramState course::predictNextProgramState(const ProgramState &state)
{
  auto newState{state};

  // Determine closest special event
  Event closestEvent = {state.producers[0].time(), 0, Event::PRODUCTION};
  for (size_t i = 1; i < state.producers.size(); ++i)
  {
    const Producer &current = state.producers[i];
    if (current.time() < closestEvent.timestamp)
    {
      closestEvent = {current.time(), i, Event::PRODUCTION};
    }
  }

  for (size_t i = 0; i < state.devices.size(); ++i)
  {
    const Device &current = state.devices[i];
    if (!current.empty() && current.time() < closestEvent.timestamp)
    {
      closestEvent = {current.time(), i, Event::COMPLETION};
    }
  }
  newState.currentTime = closestEvent.timestamp;

  if (closestEvent.type == Event::PRODUCTION)
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
  else if (closestEvent.type == Event::COMPLETION)
  {
    Device &chosenDevice = newState.devices[closestEvent.position];
    newState.accumulatedRequestTime += (chosenDevice.time() - chosenDevice.currentRequest().time());
    chosenDevice.finishProcessing();
    newState.processed_requests += 1;

    // NOTE: setRequest here works fine only if special events are prod, fin and simfin
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

std::ostream &course::printState(std::ostream &out, const ProgramState &state)
{
  // construct strings for output
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

  // output results
  out << producers << devices << "processed: " << state.processed_requests << '\n';
  out << "produced: " << state.produced_requests << '\n' << "canceled: " << state.canceled_requests << '\n';
  out << "current time: " << state.currentTime << '\n';
  out << "accumulated time: " << state.accumulatedRequestTime << '\n';
  return out;
}
