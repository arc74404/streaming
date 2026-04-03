#include "linux_capturer.hpp"

#include <pipewire/pipewire.h>

namespace stream::image
{
LinuxCapturer::LinuxCapturer() : m_registry(m_core.createRegistry())
{
    
}
} // namespace stream::image
