#include "mainLogic.hpp"
#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <exception>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include "buffer.hpp"
#include "device.hpp"
#include "disciplines.hpp"
#include "priority.hpp"
#include "producer.hpp"
#include "request.hpp"
#include "utils/idSetter.hpp"

int course::mainLogic(int argc, char **argv) { return 0; }
// namespace course
// {
//   std::chrono::milliseconds producerTimeout(500);
//   // volatile size_t stoppedProducers = 0;
//
//   const struct Config
//   {
//     deviceid_t deviceCount;
//     deviceid_t producerCount;
//     requestid_t requestCount;
//   } config{1, 3, 3};
//
//   volatile size_t producersLeft = config.producerCount;
//   // std::atomic< size_t > producersLeft = config.producerCount;
//
//   void producerThread(Producer &producer, Buffer &buffer, std::mutex &m)
//   {
//     while (producer.getRequestCount() < config.requestCount)
//     {
//       // {
//       //   std::lock_guard lock{m};
//       //   std::cout << std::this_thread::get_id() << " Producer\n";
//       // }
//       try
//       {
//         buffer.tryAddRequest(producer.produceRequest(), chooseFirstEmptyBufferSpot);
//       }
//       catch (const std::exception &e)
//       {
//         std::lock_guard lock{m};
//         std::cerr << producer.getId() << ' ' << e.what() << '\n';
//       }
//
//       std::this_thread::sleep_for(producerTimeout);
//     }
//     producersLeft--;
//   }
//
//   std::mutex deviceMtx;
//   std::condition_variable devicecv;
//   void deviceThread(Device &device, Buffer &buffer, std::mutex &m)
//   {
//     SimpleRequest req;
//     while (producersLeft > 0)
//     {
//       try
//       {
//         std::unique_lock< std::mutex > l{deviceMtx};
//         devicecv.wait(l);
//         // buffer.waitForNotEmpty();
//         buffer.tryGetRequest(req, chooseRequestFromBufferByProducerNumber);
//         device.setRequest(req);
//         device.processRequest();
//       }
//       catch (const std::exception &e)
//       {
//         std::lock_guard lock{m};
//         std::cerr << device.getId() << e.what() << " dev\n";
//       }
//     }
//   }
// } // namespace course
//
// int course::mainLogic(int argc, char **argv)
// {
//   // read configuration (mb no need for that ??)
//   size_t producerCount = course::config.producerCount;
//   size_t deviceCount = course::config.deviceCount;
//   size_t bufferSize = 3;
//
//   course::collection_t< course::Producer > producers(producerCount);
//   course::collection_t< course::Device > devices(deviceCount);
//
//   std::for_each(producers.begin(), producers.end(), course::IdSetter< course::Producer >());
//   std::for_each(devices.begin(), devices.end(), course::IdSetter< course::Device >());
//
//   Buffer buffer(bufferSize);
//
//   std::vector< std::thread > producerThreads;
//   std::mutex coutMutex;
//   std::vector< std::thread > deviceThreads;
//   for (auto &device: devices)
//   {
//     deviceThreads.emplace_back(deviceThread, std::ref(device), std::ref(buffer), std::ref(coutMutex));
//   }
//
//   for (auto &producer: producers)
//   {
//     producerThreads.emplace_back(producerThread, std::ref(producer), std::ref(buffer), std::ref(coutMutex));
//   }
//
//   // std::mutex
//   while (producerCount > 0)
//   {
//     if (!buffer.isEmpty())
//     {
//       std::unique_lock lick{deviceMtx};
//       devicecv.notify_one();
//     }
//   }
//   // for (auto &thread: producerThreads)
//   // {
//   //   thread.join();
//   // }
//   //
//   // for (auto &thread: deviceThreads)
//   // {
//   //   thread.join();
//   // }
//
//   return 0;
// }
