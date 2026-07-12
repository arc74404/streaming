#include "server.hpp"

#include <iostream>

#include "../common_consts.hpp"
#include "../common_structs/commands.hpp"
#include "../common_structs/meta_pack.hpp"
#include "../utils/generate_id.hpp"

namespace stream::server
{

Server::Server(boost::asio::io_context& context)
    : m_context(context),
      m_acceptor(context, tcp::endpoint(tcp::v4(), 9999)),
      m_udp(std::make_shared<UdpController>(context,
                                            udp::endpoint(udp::v4(), 9999)))
{
    StartAccept();
    m_udp->startUdpReceive();
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
    static auto str_handler = [](Connection& conn, const char* data)
    {
        std::cout << "string: " << (data + sizeof(structs::BaseStruct)) << '\n';
    };

    static auto screen_meta_handler = [](Connection& conn, const char* data)
    {
        const auto& meta =
            *reinterpret_cast<const structs::ScreenMetaPacket*>(data);

        size_t block_size  = consts::BYTE_PER_CHUNK;
        size_t block_count = meta.height * meta.width * 4 / block_size;
        conn.setWidth(meta.width);
        conn.setWidth(meta.height);
        conn.setupJpegBuffer(block_count, block_size);
        conn.startTimeout();
    };

    static auto command_handler = [this](Connection& conn, const char* data)
    {
        const auto& meta =
            *reinterpret_cast<const structs::CommandPacket*>(data);

        auto new_id = generateId();
        m_udp->addUser(new_id.first, conn);
        conn.sendMessage(new_id.second);
    };

    Connection::pointer new_connection = Connection::create(
        m_context,
        std::pair<structs::Type, decltype(str_handler)>(structs::Type::STRING,
                                                        str_handler),
        std::pair<structs::Type, decltype(screen_meta_handler)>(
            structs::Type::SCREEN_META_PACKET, screen_meta_handler),
        std::pair<structs::Type, decltype(command_handler)>(
            structs::Type::COMMAND, command_handler));

    auto handler =
        [this, new_connection](const boost::system::error_code& error)
    { this->handleAccept(new_connection, error); };

    m_acceptor.async_accept(new_connection->tcpSocket(), handler);
}

} // namespace stream::server
