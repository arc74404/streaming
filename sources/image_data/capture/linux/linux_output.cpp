#include "linux_output.hpp"

namespace stream::image::lin_impl
{
std::wstring
LinuxOutput::getNameImpl() const
{
    return L"none";
}
ProxyViewInVideoBuffer
LinuxOutput::captureImpl()
{
    return ProxyViewInVideoBuffer(nullptr, 0);
}

void
LinuxOutput::setId(uint32_t id)
{
    m_id = id;
}
void
LinuxOutput::setFormatId(uint32_t fid)
{
    m_format_id = fid;
}

void
LinuxOutput::init(const Registry& registry)
{
    
}
} // namespace stream::image::lin_impl
