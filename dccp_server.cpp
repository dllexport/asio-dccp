// Adapted from the server.cpp example by Chris Kohlhoff
#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include "dccp.hpp"

using boost::asio::ip::dccp;

std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

class dccp_connection
  : public boost::enable_shared_from_this<dccp_connection>
{
public:
  typedef boost::shared_ptr<dccp_connection> pointer;

  static pointer create(boost::asio::io_service& io_service)
  {
    return pointer(new dccp_connection(io_service));
  }

  dccp::socket& socket()
  {
    return socket_;
  }

  void start()
  {
    // get remote endpoint
    std::string sEndpoint = socket_.remote_endpoint().address().to_string();
    unsigned short uiClientPort = socket_.remote_endpoint().port();
    dccp::endpoint remote_endpoint = socket_.remote_endpoint();
    message_ = make_daytime_string();
#if 1
    boost::asio::async_write(socket_, boost::asio::buffer(message_),
        boost::bind(&dccp_connection::handle_write, shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
#endif
#if 0
    socket_.async_receive_from(
        boost::asio::buffer(recv_buffer_), remote_endpoint_,
        boost::bind(&udp_server::handle_receive, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
#endif
#if 0
    socket_.async_send(boost::asio::buffer(message_), remote_endpoint,
          boost::bind(&dccp_connection::handle_write, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
#endif
  }

private:
  dccp_connection(boost::asio::io_service& io_service)
    : socket_(io_service)
  {
  }

  void handle_write(const boost::system::error_code& /*error*/,
      size_t /*bytes_transferred*/)
  {
    static int i =0;
    if (i < 10)
      start();
    ++i;
  }

  dccp::socket socket_;
  std::string message_;
};

class dccp_server
{
public:
  dccp_server(boost::asio::io_service& io_service)
    : acceptor_(io_service, dccp::endpoint(dccp::v4(), 55555))
  {
    start_accept();
  }

private:
  void start_accept()
  {
    dccp_connection::pointer new_connection =
      dccp_connection::create(acceptor_.get_io_service());

    acceptor_.async_accept(new_connection->socket(),
        boost::bind(&dccp_server::handle_accept, this, new_connection,
          boost::asio::placeholders::error));
  }

  void handle_accept(dccp_connection::pointer new_connection,
      const boost::system::error_code& error)
  {
    if (!error)
    {
      new_connection->start();
    }

    start_accept();
  }

  dccp::acceptor acceptor_;
};

int main()
{
  try
  {
    boost::asio::io_service io_service;
    dccp_server server(io_service);
    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}

