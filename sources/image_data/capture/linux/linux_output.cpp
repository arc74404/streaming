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
    LinControler controler(m_wire.get());

    // LinuxOutput::ProxyBuffer buffer();
    return std::nullopt;
}

} // namespace stream::image::lin_impl
