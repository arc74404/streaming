#include "connection.hpp"

#include <iostream>

#include "../common_structs/meta_chunk.hpp"
#include "../common_structs/meta_pack.hpp"
#include "../utils/jpeg_compress.hpp"

namespace stream::server
{

void
Connection::start()
{
    std::cout << "Connection::start\n";

    doRead();

    sendMessage("Connection::start$");
}

void
Connection::doRead()
{
    auto self = shared_from_this();

    m_tcp_socket.async_read_some(
        boost::asio::buffer(m_tcp_read_buffer),
        [this, self](const boost::system::error_code& ec, std::size_t length)
        {
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

void
Connection::setupJpegBuffer(size_t block_count, size_t block_size)
{
    m_jpeg_buffer.setBlockCount(block_count);

    m_jpeg_buffer.setBlockSize(block_size);
}

void
Connection::pushChunkData(const char* chunk_data,
                          const structs::MetaChunk& meta)
{
    m_jpeg_buffer.add(chunk_data, meta.index);
}

bool
Connection::frameReady() const
{
    return m_jpeg_buffer.frameReady();
}
bool
Connection::timeout() const
{
    auto elapsed = std::chrono::steady_clock::now() - m_get_meta_time;
    double elapsed_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();

    return elapsed_ms >= m_timeout;
}

void
Connection::setTimeoutMs(double t) noexcept
{
    m_timeout = t;
}

void
Connection::startTimeout()
{
    m_get_meta_time = std::chrono::steady_clock::now();
}

std::vector<uint8_t>
Connection::compress() const
{
    return compressToJPEG(m_jpeg_buffer.data(), m_width, m_height);
}

void
Connection::setWidth(uint32_t w) noexcept
{
    m_width = w;
}

void
Connection::setHeight(uint32_t h) noexcept
{
    m_height = h;
}

} // namespace stream::server
