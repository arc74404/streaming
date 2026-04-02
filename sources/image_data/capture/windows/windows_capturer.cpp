#pragma once
#include "windows_capturer.hpp"

namespace stream::image
{
WindowsCapturer::WindowsCapturer()
{
    win_impl::ControlDevice control_device(3);
    m_outputs = 
        std::move(control_device.GetPhysicalDevice().GetOutputs());
}

bool WindowsCapturer::pickScreen(uint32_t s_in) {
    return true;
}

ProxyViewInVideoBuffer
WindowsCapturer::capture() const
{
    return ProxyViewInVideoBuffer(nullptr, 0);
}
} // namespace stream::image
