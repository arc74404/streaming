#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "screener.hpp"

#include "stb_image_write.h"

namespace stream::image
{
bool
Screener::write(const Data& data, std::string_view filename)
{
    stbi_write_png(filename.data(), data.width, data.height,
                   data.channels_count, data.data, data.row_pitch);
    return true;
}
} // namespace stream::image
