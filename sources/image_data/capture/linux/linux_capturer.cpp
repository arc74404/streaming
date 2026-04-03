#include "linux_capturer.hpp"

#include <pipewire/pipewire.h>

namespace stream::image
{
LinuxCapturer::LinuxCapturer() : Registry(), ICapturer(Registry::getOutputs())
{
}
} // namespace stream::image
