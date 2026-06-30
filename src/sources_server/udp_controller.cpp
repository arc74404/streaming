#include "udp_controller.hpp"

#include <iostream>

#include "../common_structs/meta_chunk.hpp"

namespace stream::server
{

UdpController::UdpController(boost::asio::io_context& context,
                             const udp::endpoint& endpoint)
    : m_udp_socket(context, endpoint)
{
    std::cout << "UDP socket bound to " << endpoint << std::endl;
}

void
UdpController::startUdpReceive()
{
    auto self = shared_from_this();

    m_udp_socket.async_receive_from(
        boost::asio::buffer(m_buffer), m_udp_sender_endpoint,
        [this, self](const boost::system::error_code& ec, size_t length)
        { handleUdpReceive(ec, length); });
}

void
UdpController::handleUdpReceive(const boost::system::error_code& ec,
                                size_t length)
{
    if (ec)
    {
        std::cerr << "UDP receive error: " << ec.message() << std::endl;
        startUdpReceive();
        return;
    }

    processUdpPacket(m_buffer.data(), length, m_udp_sender_endpoint);

    startUdpReceive();
}

void
UdpController::processUdpPacket(const char* data,
                                size_t length,
                                const udp::endpoint& sender_endpoint)
{
    const structs::MetaChunk* meta =
        reinterpret_cast<const structs::MetaChunk*>(data);

    size_t user_id     = meta->base.m_user_id;
    size_t chunk_index = meta->index;

    const char* chunk_data = data + sizeof(structs::MetaChunk);
    size_t chunk_size      = length - sizeof(structs::MetaChunk);

    std::cout << "User " << user_id << ", chunk " << chunk_index
              << ", size: " << chunk_size << " bytes" << std::endl;

    // auto it = m_connections_by_id.find(user_id);

    // if (it != m_connections_by_id.end())
    // {
    //     auto connection = it->second.lock();
    //     if (connection)
    //     {
    //         connection->onUdpData(chunk_data, chunk_size, *meta);
    //     }
    //     else
    //     {
    //         m_connections_by_id.erase(it);
    //     }
    // }
    // else
    // {
    //     std::cerr << "Unknown user ID: " << user_id << std::endl;
    // }
}
} // namespace stream::server
