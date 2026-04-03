#pragma once

#include "core.hpp"

#include "../interface_capturer.hpp"

#include "linux_output.hpp"
#include "registry.hpp"

namespace stream::image
{
class LinuxCapturer final : public ICapturer<lin_impl::LinuxOutput>
{
public:
    LinuxCapturer();

private:
    lin_impl::Core m_core;
    std::unique_ptr<lin_impl::Registry> m_registry;
};
} // namespace stream::image
