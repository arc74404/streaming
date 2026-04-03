#pragma once
#include "windows_capturer.hpp"

namespace stream::image
{
WindowsCapturer::WindowsCapturer()
    : win_impl::ControlDevice(3),
      ICapturer(win_impl::ControlDevice::GetPhysicalDevice().GetOutputs())
{
    ICapturer::connectWith(
        &(m_control_device.GetPhysicalDevice().GetOutputs()));
}

} // namespace stream::image
