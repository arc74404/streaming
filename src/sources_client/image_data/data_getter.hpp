#pragma once

#include <cstdint>

namespace stream::image
{
struct Data
{
    void* data;
    uint32_t row_pitch;
    uint32_t width;
    uint32_t height;
    uint32_t channels_count;
};
} // namespace stream::image
