#ifndef DISCIPLINES_HPP
#define DISCIPLINES_HPP

#include <vector>
#include "buffer.hpp"
#include "device.hpp"
#include "priority.hpp"

namespace course
{
  template < class T >
  using collection_t = std::vector< T >;

  deviceid_t chooseFirstEmptyBufferSpot(const Buffer &buff);
  size_t chooseRequestFromBufferByProducerNumber(const Buffer &buff);
  deviceid_t chooseDeviceById(const collection_t< Device > &collection);

} // namespace course
#endif
