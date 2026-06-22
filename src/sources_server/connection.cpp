#include "connection.hpp"

#include <iostream>

#include "../common_structs/meta_pack.hpp"

namespace stream::server
{
void
Connection::doRead()
{
    auto self = shared_from_this();

    m_tcp_socket.async_read_some(
        boost::asio::buffer(m_read_buffer),
        [this, self](const boost::system::error_code& ec, std::size_t length)
        {
            if (ec)
            {
                if (ec == boost::asio::error::eof)
                {
                    std::cout << "Client disconnected" << std::endl;
                }
                else
                {
                    std::cout << "Read error: " << ec.message() << std::endl;
                }
                return;
            }

            std::cout << "Received " << length << " bytes" << std::endl;

            structs::BaseStruct type_pack =
                *reinterpret_cast<structs::BaseStruct*>(m_read_buffer.data());

            std::cout << int(type_pack.m_type) << '\n';

            m_read_handlers[type_pack.m_type]->Handle(m_read_buffer.data());

            doRead();
        });
}

void
Connection::handleWrite(const boost::system::error_code& err, size_t length)
{
    if (!err)
    {
        std::cout << "Welcome message sent (" << length << " bytes)"
                  << std::endl;

        doRead();
    }
    else
    {
        std::cerr << "Failed to send welcome: " << err.message() << std::endl;
    }
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
    std::cout << "Connection::start\n";

    std::string message = "Connection::start\n";

    auto self = shared_from_this();

    auto handler =
        [self](const boost::system::error_code& ec, std::size_t length)
    { self->handleWrite(ec, length); };

    boost::asio::async_write(m_tcp_socket, boost::asio::buffer(message),
                             handler);
}

} // namespace stream::server
