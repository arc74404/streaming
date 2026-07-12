#pragma once

#include <turbojpeg.h>
#include <vector>

namespace stream
{
inline std::vector<unsigned char>
compressToJPEG(const unsigned char* pixels, int width, int height)
{
    tjhandle compressor      = tjInitCompress();
    unsigned char* jpeg_data = nullptr;
    unsigned long jpeg_size  = 0;

    tjCompress2(compressor, pixels, width, 0, height, TJPF_GRAY, &jpeg_data,
                &jpeg_size, TJSAMP_444, 85, TJFLAG_FASTDCT);

    std::vector<unsigned char> result(jpeg_data, jpeg_data + jpeg_size);
    tjFree(jpeg_data);
    tjDestroy(compressor);

    return result;
}
} // namespace stream
