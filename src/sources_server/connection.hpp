#pragma once

#include <boost/asio.hpp>

#include <memory>

using boost::asio::ip::tcp;

using boost::asio::ip::udp;

namespace stream::server
{
class Connection final : public std::enable_shared_from_this<Connection>
{
public:
    using pointer = std::shared_ptr<Connection>;

    static pointer create(boost::asio::io_context& io_context);

    tcp::socket& tcpSocket();

    udp::socket& udpSocket();

    void start();

private:
    Connection(boost::asio::io_context& io_context);

    void handleWrite(const boost::system::error_code& err,
                     size_t bytes_transferred);

    tcp::socket m_tcp_socket;
    udp::socket m_udp_socket;
};
} // namespace stream::server
