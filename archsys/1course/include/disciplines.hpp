#ifndef DISCIPLINES_HPP
#define DISCIPLINES_HPP

#include <vector>
#include "buffer.hpp"
#include "device.hpp"
#include "priority.hpp"
#include "producer.hpp"

namespace course
{
  template < class T >
  using collection_t = std::vector< T >;

  deviceid_t chooseBufferSpot(const Buffer &buff);
  deviceid_t chooseRequestFromBufferByProducerNumber(const Buffer &buff);
  deviceid_t chooseDevice(const collection_t< Device > &collection);

} // namespace course
#endif
