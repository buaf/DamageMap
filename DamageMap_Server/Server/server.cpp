#include "server.h"

Server::Server(boost::asio::io_service & ioService, short port)
  : io_service_(ioService),
    socket_(ioService, udp::endpoint(udp::v4(), port))
{
  socket_.async_receive_from(
      boost::asio::buffer(data_, max_length), sender_endpoint_,
      boost::bind(&Server::handleReceiveFrom, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

void Server::handleReceiveFrom(const boost::system::error_code & error, size_t bytes_recvd)
{
  if (!error && bytes_recvd > 0)
  {

    socket_.async_send_to(
        boost::asio::buffer(data_, bytes_recvd), sender_endpoint_,
        boost::bind(&Server::handleSendTo, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
  }
  else
  {
    socket_.async_receive_from(
        boost::asio::buffer(data_, max_length), sender_endpoint_,
        boost::bind(&Server::handleReceiveFrom, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
  }
}

void Server::handleSendTo(const boost::system::error_code & error, size_t bytesSent)
{
  socket_.async_receive_from(
      boost::asio::buffer(data_, max_length), sender_endpoint_,
      boost::bind(&Server::handleReceiveFrom, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

