#pragma once
#include "windows_capturer.hpp"

namespace stream::image
{
WindowsCapturer::WindowsCapturer()
    : ICapturer(win_impl::ControlDevice(3).GetPhysicalDevice().GetOutputs())
{
}

} // namespace stream::image
