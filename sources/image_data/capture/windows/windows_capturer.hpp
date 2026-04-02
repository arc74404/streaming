#pragma once

#include "../interface_capturer.hpp"

#include "control_device.hpp"

namespace stream::image
{
class WindowsCapturer final : private win_impl::ControlDevice,
                              public ICapturer<win_impl::WinOutput>
{
public:
    WindowsCapturer();

private:
};
} // namespace stream::image
