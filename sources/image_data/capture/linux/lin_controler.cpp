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
LinControler::fillStaging()
{
    m_wire.listenOnce();
    return true;
}

bool
LinControler::shareData(Data& data)
{
    return true;
}
} // namespace stream::image::lin_impl
