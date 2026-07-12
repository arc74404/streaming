#pragma once

#include <boost/asio.hpp>

#include <chrono>
#include <iostream>
#include <memory>
#include <queue>

#include "../common_structs/meta_chunk.hpp"
#include "../common_structs/struct_types.hpp"
#include "../utils/jpeg_farming_buffer.hpp"

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

    void start();

    void sendMessage(const std::string& message);

    void pushChunkData(const char* chunk_data, const structs::MetaChunk& meta);

    void setupJpegBuffer(size_t block_count, size_t block_size);

    bool frameReady() const;

    bool timeout() const;

    void setTimeoutMs(double t) noexcept;

    void startTimeout();

    std::vector<uint8_t> compress() const;

    void setWidth(uint32_t) noexcept;

    void setHeight(uint32_t) noexcept;

private:
    template <typename... ReadHandlers>
    Connection(boost::asio::io_context& io_context,
               std::pair<structs::Type, ReadHandlers>&&... r_handlers)
        : m_tcp_socket(io_context)
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

    std::array<char, 512> m_tcp_read_buffer;

    tcp::socket m_tcp_socket;

    struct BaseHandler
    {
        virtual void Handle(Connection& conn, const char* data) = 0;
    };
    template <typename T>
    struct Handler : BaseHandler
    {
        Handler(T&& h) : m_handler(std::forward<T>(h))
        {
        }

        void Handle(Connection& conn, const char* data) override
        {
            m_handler(conn, data);
        }
        T m_handler;
    };
    std::unordered_map<structs::Type, std::unique_ptr<BaseHandler>>
        m_read_handlers;

    void sendACK();

    void doWrite();

    std::queue<std::string> m_write_queue;

    bool m_is_writing;

    // data

    double m_timeout = 100; // ms

    std::chrono::steady_clock::time_point m_get_meta_time;

    JpegFarmingBufferOneCopy m_jpeg_buffer;

    uint32_t m_width = 0;

    uint32_t m_height = 0;
};

} // namespace stream::server
