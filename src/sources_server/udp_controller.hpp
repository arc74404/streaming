#pragma once

#include <boost/asio.hpp>

#include <array>
#include <memory>
#include <unordered_map>

#include "connection.hpp"

namespace stream::server
{

using namespace boost::asio::ip;

class UdpController final : public std::enable_shared_from_this<UdpController>
{
public:
    UdpController(boost::asio::io_context& context,
                  const udp::endpoint& endpoint);

    void addUser(size_t id, Connection& con);

    void startUdpReceive();
    void handleUdpReceive(const boost::system::error_code& ec, size_t length);
    void processUdpPacket(const char* data,
                          size_t length,
                          const udp::endpoint& sender_endpoint);

private:
    std::array<char, 1024> m_buffer;

    udp::socket m_udp_socket;

    udp::endpoint m_udp_sender_endpoint;

    std::unordered_map<size_t, Connection&> m_conns;
};
} // namespace stream::server
