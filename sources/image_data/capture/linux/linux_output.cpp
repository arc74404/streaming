#include "linux_output.hpp"

namespace stream::image::lin_impl
{

LinuxOutput::LinuxOutput(std::unique_ptr<Stream::Wire>&& wire)
    : m_wire(std::move(wire))
{
}

std::wstring
LinuxOutput::getNameImpl() const
{
    return L"name";
}

std::optional<LinuxOutput::ProxyBuffer>
LinuxOutput::captureImpl()
{
    return LinControler(*m_wire.get());
}

} // namespace stream::image::lin_impl
