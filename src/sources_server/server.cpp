#include "server.hpp"

#include <iostream>

#include "../common_structs/meta_pack.hpp"

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
    }

    StartAccept();
}

void
Server::StartAccept()
{
    static auto str_handler = [](const char* data)
    {
        std::cout << "string: " << (data + sizeof(structs::BaseStruct)) << '\n';
    };

    static auto screen_handler = [](const char* data)
    {
        const auto& meta =
            *reinterpret_cast<const structs::ScreenMetaPacket*>(data);
        std::cout << "Screen width: " << meta.width << "\n";
        std::cout << "Screen height: " << meta.height << "\n";
    };

    Connection::pointer new_connection = Connection::create(
        m_context,
        std::pair<structs::Type, decltype(str_handler)>(structs::Type::STRING,
                                                        str_handler),
        std::pair<structs::Type, decltype(screen_handler)>(
            structs::Type::SCREEN_META_PACKET, screen_handler));

    auto handler =
        [this, new_connection](const boost::system::error_code& error)
    { this->handleAccept(new_connection, error); };

    m_acceptor.async_accept(new_connection->tcpSocket(), handler);
}

} // namespace stream::server
