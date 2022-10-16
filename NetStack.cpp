/*
** EPITECH PROJECT, 2022
** rType_cpp
** File description:
** myServer
*/

#include "NetStack.hpp"

std::string MakeDaytimeString()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

// TCP

TcpStack::TcpStack(asio::io_context &ioContext, std::size_t port) : ioContext_ (ioContext),
  acceptor_(ioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 13)), port_(port)
{
  StartAccept();
}

void TcpStack::StartAccept()
{
  TcpConnection::pointer new_connection =
    TcpConnection::Create(ioContext_);

  acceptor_.async_accept(new_connection->GetSocket(),
    [this, new_connection](asio::error_code const &error) {
      this->HandleAccept(new_connection, error);
    }
    );
}

void TcpStack::HandleAccept(TcpConnection::pointer new_connection,
    const asio::error_code& error)
{
  if (!error)
  {
    new_connection->Start();
  }
  StartAccept();
}

TcpConnection::TcpConnection(asio::io_context& io_context)
  : socket_(io_context)
{
}

void TcpConnection::HandleWrite(const asio::error_code& /*error*/,
    size_t /*bytes_transferred*/)
{
}

void TcpConnection::Start()
{
  message_ = MakeDaytimeString();
  asio::async_write(socket_, asio::buffer(message_),
    std::bind(&TcpConnection::HandleWrite, shared_from_this(),
    std::placeholders::_1,
    std::placeholders::_2));
}

tcp::socket &TcpConnection::GetSocket()
{
  return socket_;
}

UdpStack::UdpStack(asio::io_context &ioContext, std::size_t port) :
  socket_(ioContext, asio::ip::udp::endpoint(asio::ip::udp::v4(), 13)), port_(port),
  ioContext_ (ioContext)
{
    StartReceive();
}

void UdpStack::StartReceive()
{
    socket_.async_receive_from(
        asio::buffer(recv_buffer_), remote_endpoint_,
        std::bind(&UdpStack::HandleReceive, this,
        std::placeholders::_1));
}

void UdpStack::HandleReceive(const asio::error_code& error)
{
  if (!error)
  {
    std::shared_ptr<std::string> message(
      new std::string(MakeDaytimeString()));
      std::cout << "j'ai recu ca : " << message->data() << std::endl;
    socket_.async_send_to(asio::buffer(*message), remote_endpoint_,
      std::bind(&UdpStack::HandleSend, this, message));
  
    StartReceive();
  }
}

void UdpStack::HandleSend(std::shared_ptr<std::string> message)
{
  std::cout << "J'ai envoyé ''" << std::endl;
}

NetStackHost::NetStackHost(asio::io_context &ioContext, std::size_t port) :
  // tcpHandler_(ioContext, port),
  udpHandler_(ioContext, port)
{
  asio::ip::udp::resolver resolver(ioContext);
  asio::ip::udp::endpoint receiver_endpoint =
    *resolver.resolve(asio::ip::udp::v4(), "127.0.0.1", "daytime").begin();
  std::string text = "";
  while (text != "exit") {
    if (text.find("send")) {
      udpHandler_.socket_.send_to(asio::buffer(std::string("test")), receiver_endpoint);
    }
    std::getline (std::cin, text);
  }
}

int main(int ac, char **av)
{
  if (ac < 2) {
    std::cout << "USAGE : bin < port >" << std::endl;
    return -1;
  }
  try {
    std::cout << std::atoi(av[1]) << std::endl;
    asio::io_context ioContext;
    NetStackHost netStack(ioContext, std::atoi(av[1]));
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}

// void NetStackHost::AcceptClient()
// {
//     TcpConnection::pointer new_connection =
//     TcpConnection::create(_ioContext);

//     _acceptor.async_accept(new_connection->socket(),
//         boost::bind(&tcp_server::handle_accept, this, new_connection,
//         asio::placeholders::error));
// }

