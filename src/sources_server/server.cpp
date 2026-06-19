#include "server.hpp"

#include <iostream>

namespace stream::server
{

Server::Server(boost::asio::io_context& context)
    : m_context(context), m_acceptor(context, tcp::endpoint(tcp::v4(), 9999))
{
    StartAccept();
}

void
Server::handleAccept(Connection::pointer new_connection,
                     const boost::system::error_code& error)
{
    if (!error)
    {
        new_connection->start();
        std::cout << "New connection!\n";
    }

    StartAccept();
}

void
Server::StartAccept()
{
    Connection::pointer new_connection = Connection::create(m_context);

    auto handler =
        [this, new_connection](const boost::system::error_code& error)
    { this->handleAccept(new_connection, error); };

    m_acceptor.async_accept(new_connection->tcpSocket(), handler);
}

} // namespace stream::server
