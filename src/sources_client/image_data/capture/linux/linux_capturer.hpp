#pragma once

#include <list>
#include <vector>

#include "core.hpp"

#include "../interface_capturer.hpp"
#include "dbus/bus_wrapper.hpp"
#include "stream/stream.hpp"

#include "linux_output.hpp"
#include "registry.hpp"

namespace stream::image
{
class LinuxCapturer final : public ICapturer<lin_impl::LinuxOutput>
{
public:
    LinuxCapturer();

    bool prepare() noexcept override;

    uint32_t m_picked_index = 0;

private:
    bool start() noexcept;

    dbus::DBusWrapper m_bus;
    lin_impl::Core m_core;

    bool setupOutputs();
    std::vector<lin_impl::Stream> m_streams;
    std::vector<lin_impl::LinuxOutput> m_outputs;
};
} // namespace stream::image
