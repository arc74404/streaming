#pragma once

#include "../interface_capturer.hpp"

#include "control_device.hpp"

namespace stream::image
{
class WindowsCapturer final : public ICapturer<win_impl::WinOutput>
{
public:
    WindowsCapturer();

    bool prepare() noexcept override;

private:
    std::vector<win_impl::WinOutput> m_screens;
    win_impl::ControlDevice m_control_device;
};
} // namespace stream::image
