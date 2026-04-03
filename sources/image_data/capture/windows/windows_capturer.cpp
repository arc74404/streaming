#pragma once
#include "windows_capturer.hpp"

namespace stream::image
{
WindowsCapturer::WindowsCapturer() : m_control_device(3)
{
    ICapturer::connectWith(
        &(m_control_device.GetPhysicalDevice().GetOutputs()));
}

} // namespace stream::image
