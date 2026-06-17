#include "sender.hpp"

#include <iostream>

namespace stream::io
{

constexpr Sender&
Sender::setPacketSize(uint32_t packet_size) noexcept
{
    m_packet_size = packet_size;
    return *this;
}

Sender::Sender(std::string_view server_name_or_ip, uint32_t port)
    : m_udp_socket(m_io_context), m_tcp_socket(m_io_context)
{
    udp::resolver udp_resolver(m_io_context);

    m_udp_server_endpoint =
        *udp_resolver
             .resolve(udp::v4(), server_name_or_ip, std::to_string(port))
             .begin();

    m_udp_socket.open(udp::v4());

    std::string message = "Hellow server\n";
    m_udp_socket.send_to(boost::asio::buffer(message), m_udp_server_endpoint);

    tcp::resolver tcp_resolver(m_io_context);
    tcp::resolver::results_type tcp_endpoints =
        tcp_resolver.resolve(server_name_or_ip, std::to_string(port));

    boost::asio::connect(m_tcp_socket, tcp_endpoints);

    boost::asio::write(m_tcp_socket, boost::asio::buffer("Hello, TCP server!"));
}

void
Sender::sendFrame(const image::Data& data)
{
    sendMeta(data);
    // m_socket.send_to(boost::asio::buffer(message), m_server_endpoint);
}

void
Sender::sendMeta(const image::Data& data)
{
    // MetaPacket meta;
    // boost::asio::write(m_tcp_socket, boost::asio::buffer(&meta,
    // sizeof(meta)));
}

} // namespace stream::io
