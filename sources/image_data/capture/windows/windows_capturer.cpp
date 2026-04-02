#pragma once
#include "windows_capturer.hpp"

namespace stream::image
{
WindowsCapturer::WindowsCapturer() : m_control_device(3)
{
}
ProxyViewInVideoBuffer
WindowsCapturer::captureScreen() const
{
    return ProxyViewInVideoBuffer(nullptr, 0);
}
} // namespace stream::image
