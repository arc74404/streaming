#include "lin_controler.hpp"

namespace stream::image::lin_impl
{
bool
LinControler::updateDataOnGui()
{
    return false;
}

bool
LinControler::fillStaging(pw_buffer& buffer)
{
    return true;
}

bool
LinControler::shareData(pw_buffer& buffer, Data& data)
{
    return true;
}
} // namespace stream::image::lin_impl
