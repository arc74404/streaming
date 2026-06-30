#pragma once

#include <boost/asio.hpp>

#include "connection.hpp"
#include "udp_controller.hpp"

using boost::asio::ip::tcp;

namespace stream::server
{
class Server final
{
public:
    Server(boost::asio::io_context& context);

private:
    void StartAccept();
    void handleAccept(Connection::pointer new_connection,
                      const boost::system::error_code& error);

    boost::asio::io_context& m_context;
    std::shared_ptr<UdpController> m_udp;
    tcp::acceptor m_acceptor;
};
} // namespace stream::server
