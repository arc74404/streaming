#pragma once

#include "../capturer.hpp"
#include "linux_output.hpp"

namespace stream::image
{
class LinuxCapturer final : public ICapturer<LinuxOutput> {
public:
    LinuxCapturer();
private:

};
}