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

    auto active_buffer = capturer.capture(0);

    image::Screener screener;

    if (active_buffer.has_value())
    {
        image::Data data;

        auto& buf = active_buffer.value();

        buf.listenOn();

        int iter_count = 0;

        while (iter_count < 5)
        {
            // std::cout << "iter_count: " << iter_count << '\n';
            if (false == buf.get(data))
            {
                std::cout << "continue\n";
                continue;
            }
            ++iter_count;

            screener.write(data,
                           "screen_" + std::to_string(iter_count) + ".png");

            //     // std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    return 0;
}
