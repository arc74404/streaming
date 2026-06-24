#pragma once

#include <boost/asio.hpp>

#include <mutex>
#include <optional>

#include "../image_data/data_getter.hpp"

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

namespace stream::io
{
struct Packet final
{
};

class Sender final
{
public:
    Sender(std::string_view server_name_or_ip, uint32_t port);

    constexpr Sender& setPacketSize(uint32_t packet_size) noexcept;

    void sendFrames(
        std::function<std::optional<image::Data>()>&& send_frame_callback);

    void run();

private:
    void sendMetaTcp(const image::Data& data);

    void sendDataUdp(const image::Data& data);

    void handleReadTcp(const boost::system::error_code& ec, size_t length);
    void doReadTcp();

    void waitForAck();

    // Chunks cutter //

    ChunksCutter m_cutter;

    //////////////

    uint32_t m_packet_size;

    boost::asio::io_context m_io_context;

    // udp for data

    udp::endpoint m_udp_server_endpoint;

    udp::socket m_udp_socket;

    // tcp for meta

    std::condition_variable m_ack_cv;
    mutable std::mutex m_ack_mutex;
    bool m_ack_received;

    std::array<char, 512> m_tcp_response_buf;

    tcp::socket m_tcp_socket;

    std::function<std::optional<image::Data>()> m_send_frame_callback;
};
} // namespace stream::io
