#include "disciplines.hpp"
#include <cstddef>
#include <stdexcept>
#include "buffer.hpp"
#include "priority.hpp"
#include "request.hpp"

course::deviceid_t course::chooseFirstEmptyBufferSpot(const Buffer &buff)
{
  deviceid_t spotId = 0;
  for (; spotId < buff.size(); ++spotId)
  {
    if (buff[spotId].empty())
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

size_t course::chooseRequestFromBufferByProducerNumber(const Buffer &buff)
{
  SimpleRequest chosenRequest(EMPTY, 0);
  size_t pos = 0;

  for (size_t i = 0; i < buff.size(); ++i)
  {
    if ((!buff[i].empty() && chosenRequest > buff[i]) || (!buff[i].empty() && chosenRequest.empty()))
    {
      chosenRequest = buff[i];
      pos = i;
    }
  }

  if (chosenRequest == SimpleRequest(EMPTY, 0))
  {
    throw std::runtime_error("No requests in buffer");
  }

  return pos;
}

course::deviceid_t course::chooseDeviceById(const collection_t< Device > &collection)
{
  deviceid_t deviceId = 0;
  for (; deviceId < collection.size(); ++deviceId)
  {
    if (collection[deviceId].empty())
    {
      break;
    }
  }
  if (deviceId == collection.size())
  {
    throw std::runtime_error("No empty devices");
  }
  return deviceId;
}
