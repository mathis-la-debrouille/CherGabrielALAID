/*
** EPITECH PROJECT, 2022
** rType_cpp
** File description:
** myServer
*/


#include <iostream>
#include <asio.hpp>
#include <vector>
#include <iostream>
#include <string>
#include <variant>
#include <ctime>
#include <functional>

using asio::ip::tcp;

enum DataType 
{
    other,
    string,
    number,
    serialNumber
};

struct NetHeader
{
  std::string name;
  std::size_t status;
};

class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
public:

  typedef std::shared_ptr<TcpConnection> pointer;
  static pointer Create(asio::io_context& io_context)
  {
    return pointer(new TcpConnection(io_context));
  }

  tcp::socket& GetSocket();
  void Start();

  private:
  TcpConnection(asio::io_context& io_context);
  void HandleWrite(const asio::error_code& /*error*/, size_t /*bytes_transferred*/);

  tcp::socket socket_;
  std::string message_;
};



class TcpStack
{
  public:
    TcpStack(asio::io_context &ioContext, std::size_t port);
    ~TcpStack() = default;

  private:
    void StartAccept();
    void HandleAccept(TcpConnection::pointer new_connection, const asio::error_code& error);

    asio::io_context &ioContext_;
    asio::ip::tcp::acceptor acceptor_;
    std::size_t port_;
};



class UdpStack
{
  public:
    UdpStack(asio::io_context &ioContext, std::size_t port);
    ~UdpStack() = default;
    void StartReceive();
    void HandleReceive(const asio::error_code& error);
    void HandleSend(std::shared_ptr<std::string> message);

    asio::ip::udp::socket socket_;
  private:
    asio::io_context &ioContext_;
    asio::ip::udp::endpoint remote_endpoint_;
    std::array<char, 1> recv_buffer_;
    std::size_t port_;
};

class NetStackHost
{
    public:
        /**
         * @brief Will construct an acceptor, set an ioContext and will generate
         *     ** a hashCode used to communicate for UDP. \n
         *     ** Will also start a TCP and UDP connection with acceptor and receiver 
         * 
         * @param ioContext 
         */
        NetStackHost(asio::io_context &ioContext, std::size_t port);
        ~NetStackHost() = default;

        void AcceptClient();

    protected:

    private:
        void TcpConnect();

        // TcpStack tcpHandler_;
        UdpStack udpHandler_;

        std::vector<short> hashCode_;
        NetHeader header_;
};
