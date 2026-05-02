

#include "main_logic.hpp"

#include <boost/asio.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <ctime>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;
std::string
make_string(boost::asio::streambuf& streambuf)
{
  return { boost::asio::buffers_begin(streambuf.data()),
           boost::asio::buffers_end(streambuf.data()) };
}

int
total_mail::smtp::main_logic(int argc, char** argv)
{
  try
  {
    //    std::string ip_address = "127.0.0.1";
    // boost::asio::ip::tcp::endpoint endpoint(
    // boost::asio::ip::make_address_v4(ip_address), port);
    boost::asio::io_context context;

    std::string ip_address = "localhost";
    std::string port = "8080";

    tcp::resolver resolver(context);

    auto endpoints = resolver.resolve(ip_address, port);

    boost::asio::ip::tcp::socket socket(context);
    boost::asio::connect(socket, endpoints);

    boost::asio::streambuf read_buffer;
    boost::system::error_code ignored_error;
    boost::asio::read(
      socket, read_buffer, boost::asio::transfer_all(), ignored_error);
    std::cout << make_string(read_buffer) << '\n';
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
// std::string
// make_string(boost::asio::streambuf& streambuf)
// {
//   return { boost::asio::buffers_begin(streambuf.data()),
//            boost::asio::buffers_end(streambuf.data()) };
// }
//
// int
// total_mail::smtp::main_logic(int argc, char** argv)
// {
//   try
//   {
//     std::string ip_address = "127.0.0.1";
//     unsigned short port = 8080;
//     boost::asio::ip::tcp::endpoint endpoint(
//       boost::asio::ip::make_address_v4(ip_address), port);
//     boost::asio::io_context context;
//
//     boost::asio::ip::tcp::socket socket(context);
//     socket.connect(endpoint);
//
//     boost::asio::streambuf read_buffer;
//     boost::system::error_code ignored_error;
//     boost::asio::read(
//       socket, read_buffer, boost::asio::transfer_all(), ignored_error);
//     std::cout << make_string(read_buffer) << '\n';
//   }
//   catch (std::exception& e)
//   {
//     std::cerr << e.what() << std::endl;
//   }
//
//   return 0;
// }
