#include "lin_controler.hpp"

namespace stream::image::lin_impl
{

LinControler::LinControler(Stream::Wire& wire) : m_wire(wire)
{
}

bool
LinControler::updateDataOnGui()
{
    return false;
}

bool
LinControler::fillStaging(pw_buffer& buffer)
{
    m_wire.listen();
    return true;
}

bool
LinControler::shareData(pw_buffer& buffer, Data& data)
{
    return true;
}
} // namespace stream::image::lin_impl
