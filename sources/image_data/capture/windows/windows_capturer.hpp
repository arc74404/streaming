#pragma once

#include "../capturer.hpp"

#include "control_device.hpp"

namespace stream::image
{
class WindowsCapturer final
{
public:
    WindowsCapturer();

    ProxyViewInVideoBuffer captureScreen() const;

private:
    win_impl::ControlDevice m_control_device;
};
} // namespace stream::image
