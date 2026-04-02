#pragma once

#include "../interface_capturer.hpp"

#include "linux_output.hpp"
#include "registry.hpp"

namespace stream::image
{
class LinuxCapturer final : private lin_impl::Registry,
                            public ICapturer<lin_impl::LinuxOutput>
{
public:
    LinuxCapturer();

private:
};
} // namespace stream::image
