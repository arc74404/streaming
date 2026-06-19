#pragma once

#include <boost/asio.hpp>

#include "connection.hpp"

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

    tcp::acceptor m_acceptor;
};
} // namespace stream::server
