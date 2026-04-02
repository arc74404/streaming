#pragma once

#include "../interface_capturer.hpp"

#include "linux_output.hpp"

namespace stream::image
{
class LinuxCapturer final : public ICapturer<lin_impl::LinuxOutput>
{
public:
    LinuxCapturer();

private:
};
} // namespace stream::image
