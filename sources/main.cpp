#include <iostream>

#include "image_data/capture/capturer.hpp"
#include "image_data/capture/windows/windows_capturer.hpp"

using namespace stream::image;

using Capturer = CapturerT<WindowsCapturer>;

int
main()
{
    Capturer capturer;
}
