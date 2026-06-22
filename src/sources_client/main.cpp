#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

#include "../utils/global_params.hpp"
#include "image_data/capturer.hpp"
#include "io/sender.hpp"
#include "utils/write.hpp"

using namespace stream;

int
main()
{
    std::cout << "Start\n";

    try
    {
        GlobalParams<int, std::string> global_params;
        global_params.addParam<int>("first", 4);
        global_params.addParam<std::string>("second", "cat");

        std::setlocale(LC_ALL, "");
        image::Capturer capturer;

        if (false == capturer.prepare())
        {
            return 1;
        }

        auto&& info = capturer.screensInfo();

        auto active_buffer = capturer.capture(0);

        io::Sender sender("localhost", 9999);

        if (active_buffer.has_value())
        {
            image::Data data;

            auto& buf = active_buffer.value();

            int iter_count = 0;

            while (iter_count < 5)
            {
                buf.load();

                if (false == buf.get(data))
                {
                    continue;
                }

                ++iter_count;

                sender.sendFrame(data);
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
    return 0;
}
