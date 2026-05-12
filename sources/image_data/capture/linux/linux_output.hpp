#pragma once

#include <pipewire/pipewire.h>
#include <spa/param/video/raw.h>

#include <cstdint>
#include <optional>

#include "../ioutput.hpp"
#include "stream/stream.hpp"

#include "lin_controler.hpp"

namespace stream::image::lin_impl
{

class Registry;

class LinuxOutput final : public IOutput<LinuxOutput>
{
public:
    using ProxyBuffer = ProxyViewInVideoBuffer<pw_buffer, LinControler>;

    LinuxOutput(std::unique_ptr<Stream::Wire>&& listener);

    std::wstring getNameImpl() const;

    std::optional<ProxyBuffer> captureImpl();

private:
    std::unique_ptr<Stream::Wire> m_wire;
};
} // namespace stream::image::lin_impl
