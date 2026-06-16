#include "sender.hpp"

namespace stream::io
{
void
Sender::sendToServer(const image::Data& data) const
{
    udp::resolver resolver(m_io_context);
    udp::endpoint receiver_endpoint =
        *resolver.resolve(udp::v4(), argv[1], "daytime").begin();
}
} // namespace stream::io
