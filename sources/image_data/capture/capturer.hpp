#pragma once

#include <memory>

#include "../buf/proxy_view_in_video_buffer.hpp"

namespace stream::image
{

template <typename T>
concept CapturerInterface =
    requires(T& t) { ProxyViewInVideoBuffer(t.captureScreen()); };

template <CapturerInterface C>
class CapturerT
{
public:
    ProxyViewInVideoBuffer captureScreen() const
    {
        m_pimpl->captureScreen();
    }

private:
    std::unique_ptr<C> m_pimpl;
};
} // namespace stream::image
