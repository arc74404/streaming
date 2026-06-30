#include "sender.hpp"

#include <iostream>
#include <ranges>
#include <thread>

#include "../../common_structs/commands.hpp"
#include "../../common_structs/meta_pack.hpp"
#include "../../common_structs/string_pack.hpp"
#include "../../utils/cut_on_chunks.hpp"

namespace stream::io
{
Sender::Sender(std::string_view server_name_or_ip, uint32_t port)
    : m_udp_socket(m_io_context), m_tcp_socket(m_io_context)
{
    udp::resolver udp_resolver(m_io_context);
    m_udp_server_endpoint =
        *udp_resolver
             .resolve(udp::v4(), server_name_or_ip, std::to_string(port))
             .begin();

    m_udp_socket.open(udp::v4());

    std::string message = "Hello server\n";
    m_udp_socket.send_to(boost::asio::buffer(message), m_udp_server_endpoint);

    // tcp

    tcp::resolver tcp_resolver(m_io_context);
    tcp::resolver::results_type tcp_endpoints =
        tcp_resolver.resolve(server_name_or_ip, std::to_string(port));

    boost::asio::connect(m_tcp_socket, tcp_endpoints);

    structs::CommandPacket command_pack =
        structs::createCommand(structs::Command::GET_ID);

    boost::asio::write(
        m_tcp_socket, boost::asio::buffer(&command_pack, sizeof(command_pack)));

    doReadTcp();

    std::thread(
        [this]()
        {
            auto work = boost::asio::make_work_guard(m_io_context);
            m_io_context.run();
        })
        .detach();

    waitForId();

    std::cout << "GET ID: " << m_id << '\n';
}

void
Sender::sendFrames(
    std::function<std::optional<image::Data>()>&& send_frame_callback)
{
    m_send_frame_callback = std::move(send_frame_callback);
}

void
Sender::parseResponse(std::string_view response)
{
    auto splited = std::views::split(response, "$");

    for (auto&& r : splited)
    {
        std::string_view rr(r.begin(), r.end());

        std::cout << rr << '\n';

        if (rr == "ACK$")
        {
            {
                std::lock_guard<std::mutex> lock(m_ack_mutex);
                m_ack_received = true;
            }
            m_ack_cv.notify_one();
        }
        else if (rr.find("id: ") != std::string::npos)
        {
            {
                std::lock_guard<std::mutex> lock(m_id_mutex);
                m_id = std::stoull(std::string(rr.begin() + 4, rr.end()));
                m_id_received = true;
            }
            m_id_cv.notify_one();
        }
    }
}

void
Sender::handleReadTcp(const boost::system::error_code& ec, size_t length)
{
    if (ec)
    {
        std::cerr << "TCP Read error: " << ec.message() << std::endl;
        return;
    }

    std::string response(m_tcp_response_buf.data(), length);
    std::cout << "Server response: " << response << std::endl;

    parseResponse(response);

    doReadTcp();
}

void
Sender::doReadTcp()
{
    m_tcp_socket.async_read_some(
        boost::asio::buffer(m_tcp_response_buf),
        [this](const boost::system::error_code& ec, size_t length)
        { handleReadTcp(ec, length); });
}

void
Sender::waitForAck()
{
    std::unique_lock<std::mutex> lock(m_ack_mutex);

    m_ack_cv.wait(lock, [this] { return m_ack_received; });

    m_ack_received = false;
}

void
Sender::waitForId()
{
    std::unique_lock<std::mutex> lock(m_id_mutex);

    m_id_cv.wait(lock, [this] { return m_id_received; });
}

void
Sender::run()
{
    if (!m_send_frame_callback)
    {
        std::cerr << "Error: send_frame_callback is not set!" << std::endl;
        return;
    }

    while (true)
    {
        std::optional<image::Data> data = m_send_frame_callback();

        if (false == data.has_value())
        {
            continue;
        }
        sendMetaTcp(data.value());

        sendDataUdp(data.value());

        waitForAck();
    }
}

void
Sender::sendDataUdp(const image::Data& data)
{
    auto&& result = cutOnChunks(static_cast<size_t>(data.height) *
                                    static_cast<size_t>(data.row_pitch),
                                static_cast<const char*>(data.data), 512);

    for (const auto& chunk_buffers : result.buffers)
    {
        m_udp_socket.send_to(chunk_buffers, m_udp_server_endpoint);
    }
}

void
Sender::sendMetaTcp(const image::Data& data)
{
    structs::ScreenMetaPacket meta(0);
    meta.height = data.height;
    meta.width  = data.width;

    boost::asio::write(m_tcp_socket, boost::asio::buffer(&meta, sizeof(meta)));
}

} // namespace stream::io
