#include "device.hpp"

course::Device::Device() : deviceId_(0), processedRequestsCount_(0) {}

course::Device::Device(deviceid_t id) : deviceId_(id), processedRequestsCount_(0) {}

void course::Device::processRequest(const SimpleRequest &request) { ++processedRequestsCount_; }
