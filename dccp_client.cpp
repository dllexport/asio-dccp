#include <iostream>
#include <boost/array.hpp>
// NOTE: this code was compiled with boost version 1.53
#include <boost/asio.hpp>

#include "dccp.hpp"

using boost::asio::ip::dccp;

/**
 * DCCP test client:
 * This program connects to the specified host on dccp port 55555 
 * and reads data until there is no more data to be read and then exits.
 */
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
    dccp::endpoint ep = *resolver.resolve(query);

    dccp::socket socket(io_service);
//    socket.connect(dccp::endpoint(dccp::v4(), 55555));
    socket.connect(ep);

    for (;;)
    {
      boost::array<char, 128> buf;
      boost::system::error_code error;

      size_t len = socket.read_some(boost::asio::buffer(buf), error);

      // TODO: need to determine why eof is not generated when the server closes the connection 
      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.

      // HACK until connection issue is resolved.
      if (len == 0) 
      {
        socket.close();
        break;
      }

      std::cout.write(buf.data(), len);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}

