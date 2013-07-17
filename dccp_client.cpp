#include <iostream>
#include <boost/array.hpp>
// NOTE: this code was compiled with boost version 1.53
#include <boost/asio.hpp>

#include "dccp.hpp"

using boost::asio::ip::dccp;

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: client <host>" << std::endl;
      return 1;
    }

    boost::asio::io_service io_service;

    dccp::resolver resolver(io_service);
    dccp::resolver::query query(argv[1], "55555");

    dccp::socket socket(io_service);
    socket.connect(dccp::endpoint(dccp::v4(), 55555));

    for (;;)
    {
      boost::array<char, 128> buf;
      boost::system::error_code error;

      size_t len = socket.read_some(boost::asio::buffer(buf), error);

      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.

      std::cout.write(buf.data(), len);
   }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}

