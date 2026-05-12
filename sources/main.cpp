#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

#include "image_data/capturer.hpp"
#include "image_data/screener.hpp"

using namespace stream;

int
main()
{
    std::setlocale(LC_ALL, "");
    image::Capturer capturer;

    if (false == capturer.prepare())
    {
        return 1;
    }
    auto&& info = capturer.screensInfo();

    for (auto&& r : info)
    {
        std::wcout << r.name << '\n';
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));

    auto active_buffer = capturer.capture(0);

    image::Screener screener;

    if (active_buffer.has_value())
    {
        image::Data data;

        auto& buf = active_buffer.value();

        int iter_count = 0;

        while (iter_count++ < 5)
        {
            buf.fillStaging(data);

            screener.write(data,
                           "screen_" + std::to_string(iter_count) + ".png");

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    return 0;
}
