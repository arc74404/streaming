#include "linux_output.hpp"

namespace stream::image::lin_impl
{

LinuxOutput::LinuxOutput(std::unique_ptr<Listener<pw_stream_events>>&& listener)
    : m_listener(std::move(listener))
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
    return std::nullopt;
}

} // namespace stream::image::lin_impl
