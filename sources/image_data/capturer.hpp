#pragma once

#ifdef _WIN32
#    include "capture/windows/windows_capturer.hpp"
#elif __linux__
#    include "capture/linux/linux_capturer.hpp"
#endif //_WIN32

namespace stream::image
{
#ifdef _WIN32
using Capturer = WindowsCapturer;
#elif __linux__
using Capturer = LinuxCapturer;
#endif //_WIN32
} // namespace stream::image
