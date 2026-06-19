#include "connection.hpp"

namespace stream::server
{
Connection::Connection(boost::asio::io_context& io_context)
    : m_tcp_socket(io_context), m_udp_socket(io_context)
{
}

void
Connection::handleWrite(const boost::system::error_code&, size_t)
{
}

Connection::pointer
Connection::create(boost::asio::io_context& io_context)
{
    return pointer(new Connection(io_context));
}
tcp::socket&
Connection::tcpSocket()
{
    return m_tcp_socket;
}
udp::socket&
Connection::udpSocket()
{
    return m_udp_socket;
}

void
Connection::start()
{
    std::string message = "Connection::start\n";

    auto handler = [this](const boost::system::error_code& ec,
                          std::size_t length) { handleWrite(ec, length); };

    boost::asio::async_write(m_tcp_socket, boost::asio::buffer(message),
                             handler);
}

} // namespace stream::server
