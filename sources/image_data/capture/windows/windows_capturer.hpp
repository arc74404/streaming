#pragma once

#include "../capturer.hpp"

#include "control_device.hpp"

namespace stream::image
{
class WindowsCapturer final : public ICapturer<win_impl::WinOutput>
{
public:
    WindowsCapturer();

private:
};
} // namespace stream::image
