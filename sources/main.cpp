#include <iostream>
#include <string>

#include "image_data/capturer.hpp"
using namespace stream::image;

// using Capturer = CapturerT<WindowsCapturer>;

int
main()
{
    Capturer capturer;
    auto&& info = capturer.screensInfo();
    for (auto&& r : info)
    {
        std::wcout << r.name << '\n';
    }
    if (capturer.pickScreen(0))
    {
        capturer.capture();
    }
}
