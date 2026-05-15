#pragma once

#include <pipewire/pipewire.h>
#include <spa/param/video/raw.h>

#include <cstdint>
#include <optional>

#include "../../buf/data_getter.hpp"
#include "../ioutput.hpp"
#include "stream/stream.hpp"

namespace stream::image::lin_impl
{

class Registry;

class LinuxOutput final : public IOutput<LinuxOutput>
{
public:
    struct ProxyBuffer
    {
    public:
        ProxyBuffer(Wire& wire) : m_wire(wire)
        {
        }
        void listenOn()
        {
            m_wire.listenOn();
        }
        bool get(image::Data& getter) const
        {
            return m_wire.share(getter);
        }

    private:
        Wire& m_wire;
    };

    LinuxOutput(std::unique_ptr<Wire>&& listener);

    std::wstring getNameImpl() const;

    std::optional<ProxyBuffer> captureImpl();

private:
    std::unique_ptr<Wire> m_wire;
};
} // namespace stream::image::lin_impl
