#include "connection.hpp"

#include <iostream>

#include "../common_structs/meta_chunk.hpp"
#include "../common_structs/meta_pack.hpp"

namespace stream::server
{

void
Connection::start()
{
    std::cout << "Connection::start\n";

    doRead();

    sendMessage("Connection::start\n");
}

void
Connection::doRead()
{
    auto self = shared_from_this();

    m_tcp_socket.async_read_some(
        boost::asio::buffer(m_tcp_read_buffer),
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

            if (length >= sizeof(structs::BaseStruct))
            {
                const auto* base_pack =
                    reinterpret_cast<const structs::BaseStruct*>(
                        m_tcp_read_buffer.data());
                structs::Type packet_type = base_pack->m_type;

                std::cout << "Packet type: " << static_cast<int>(packet_type)
                          << '\n';

                if (m_read_handlers.contains(packet_type))
                {
                    m_read_handlers[packet_type]->Handle(
                        *this, m_tcp_read_buffer.data());
                }
            }

            sendACK();

            doRead();
        });
}

void
Connection::sendMessage(const std::string& message)
{
    m_write_queue.push(message + "$");

    if (false == m_is_writing)
    {
        doWrite();
    }
}

void
Connection::doWrite()
{
    if (m_write_queue.empty())
    {
        m_is_writing = false;
        return;
    }

    m_is_writing = true;

    const std::string& message = m_write_queue.front();

    auto self = shared_from_this();

    boost::asio::async_write(
        m_tcp_socket, boost::asio::buffer(message),
        [this, self](const boost::system::error_code& err, size_t length)
        { this->handleWrite(err, length); });
}

void
Connection::handleWrite(const boost::system::error_code& err, size_t length)
{
    if (!err)
    {
        if (false == m_write_queue.empty())
        {
            m_write_queue.pop();
        }

        doWrite();
    }
    else
    {
        std::cerr << "Write error: " << err.message() << std::endl;
        m_is_writing = false;
    }
}

void
Connection::sendACK()
{
    sendMessage("ACK");
}

tcp::socket&
Connection::tcpSocket()
{
    return m_tcp_socket;
}
} // namespace stream::server
