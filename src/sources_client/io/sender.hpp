#pragma once

#include <boost/asio.hpp>

#include "../image_data/data_getter.hpp"

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

namespace stream::io
{
struct Packet final
{
};

struct MetaPacket final
{
    uint32_t height;
    uint32_t width;
    uint32_t packet_size;
};

class Sender final
{
public:
    Sender(std::string_view server_name_or_ip, uint32_t port);

    constexpr Sender& setPacketSize(uint32_t packet_size) noexcept;

    void sendFrame(const image::Data& data);

private:
    void sendMeta(const image::Data& data);

    uint32_t m_packet_size;

    boost::asio::io_context m_io_context;

    // udp for data

    udp::endpoint m_udp_server_endpoint;

    udp::socket m_udp_socket;

    // tcp for meta
    tcp::socket m_tcp_socket;
};
} // namespace stream::io
