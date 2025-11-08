#include "disciplines.hpp"
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <utility>
#include "buffer.hpp"
#include "priority.hpp"
#include "request.hpp"

course::deviceid_t course::chooseBufferSpot(const Buffer &buff)
{
  size_t spotId = 0;
  // std::for_each(buff.begin(), buff.end(), [&](const auto & item) -> void { })
  for (; spotId < buff.size(); ++spotId)
  {
    if (isEmpty(buff[spotId]))
    {
      break;
    }
  }
  if (spotId == buff.size())
  {
    throw std::runtime_error("No empty slots in buffer");
  }
  return spotId;
}

course::deviceid_t course::chooseRequestFromBufferByProducerNumber(const Buffer &buff)
{
  std::pair< size_t, deviceid_t > chosenRequest{0, EMPTY};
  for (size_t i = 0; i < buff.size(); ++i)
  {
    if ()
  }
}

course::deviceid_t course::chooseDevice(const collection_t< Device > &collection) { return 0; }
