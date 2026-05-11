#pragma once

#include <string>

#include "../buf/proxy_view_in_video_buffer.hpp"

#include "screen_info.hpp"

namespace stream::image
{
template <typename Derived>
class IOutput
{
public:
    operator ScreenInfo() const
    {
        return getInfo();
    }
    ScreenInfo getInfo() const
    {
        return ScreenInfo{.name =
                              static_cast<const Derived*>(this)->getNameImpl()};
    }

    decltype(auto) capture()
    {
        return static_cast<Derived*>(this)->captureImpl();
    }

private:
};
} // namespace stream::image
