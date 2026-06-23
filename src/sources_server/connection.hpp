#pragma once

#include <boost/asio.hpp>

#include <iostream>
#include <memory>
#include <queue>

#include "../common_structs/struct_types.hpp"

using boost::asio::ip::tcp;

using boost::asio::ip::udp;

namespace stream::server
{
class Connection final : public std::enable_shared_from_this<Connection>
{
public:
    using pointer = std::shared_ptr<Connection>;

    template <typename... ReadHandlers>
    static pointer create(
        boost::asio::io_context& io_context,
        std::pair<structs::Type, ReadHandlers>&&... r_handlers)
    {
        return pointer(new Connection(io_context, std::move(r_handlers)...));
    }

    tcp::socket& tcpSocket();
    udp::socket& udpSocket();

    void start();

private:
    template <typename... ReadHandlers>
    Connection(boost::asio::io_context& io_context,
               std::pair<structs::Type, ReadHandlers>&&... r_handlers)
        : m_tcp_socket(io_context), m_udp_socket(io_context)
    {
        auto add_handler = [this]<typename T>(std::pair<structs::Type, T>&& h)
        {
            m_read_handlers[h.first] =
                std::make_unique<Handler<T>>(std::move(h.second));
        };

        (add_handler(std::move(r_handlers)), ...);
    }

    void handleWrite(const boost::system::error_code& err,
                     size_t bytes_transferred);
    void doRead();

    std::array<char, 512> m_read_buffer;

    tcp::socket m_tcp_socket;
    udp::socket m_udp_socket;

    struct BaseHandler
    {
        virtual void Handle(const char* data) = 0;
    };
    template <typename T>
    struct Handler : BaseHandler
    {
        Handler(T&& h) : m_handler(std::forward<T>(h))
        {
        }

        void Handle(const char* data) override
        {
            m_handler(data);
        }
        T m_handler;
    };

    void sendACK();

    std::unordered_map<structs::Type, std::unique_ptr<BaseHandler>>
        m_read_handlers;

    void doWrite();

    void sendMessage(const std::string& message);

    std::queue<std::string> m_write_queue;
    bool m_is_writing;
};

} // namespace stream::server
