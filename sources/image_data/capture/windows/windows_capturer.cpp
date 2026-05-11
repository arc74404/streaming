#pragma once
#include "windows_capturer.hpp"

#include <ranges>

namespace stream::image
{
WindowsCapturer::WindowsCapturer() : m_control_device(3)
{
}

bool
WindowsCapturer::prepare() noexcept
{
    m_screens = m_control_device.getScreens();
    ICapturer::connectWith(&m_screens);
    return true;
}

} // namespace stream::image
