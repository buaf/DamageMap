#ifndef SERVER_H
#define SERVER_H

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

class Server
{
public:
  Server(boost::asio::io_service & ioService, short port);

  void sendDataTo();
  void handleReceiveFrom(const boost::system::error_code & error, size_t bytes_recvd);
  void handleSendTo(const boost::system::error_code & error, size_t bytesSent);

private:
  boost::asio::io_service & io_service_;

  udp::socket socket_;
  udp::endpoint sender_endpoint_;

  enum { max_length = 1024 };
  char data_[max_length];
};

#endif // SERVER_H
