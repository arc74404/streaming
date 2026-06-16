#pragma once

#include <boost/asio.hpp>

#include "../image_data/data_getter.hpp"

using boost::asio::ip::udp;

namespace stream::io
{

class Sender final
{
public:

    void sendToServer(const image::Data& data) const;

private:
    boost::asio::io_context m_io_context;
};
} // namespace stream::io
