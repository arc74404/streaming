#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

#include "../utils/global_params.hpp"
#include "../utils/write.hpp"
#include "image_data/capturer.hpp"
#include "io/sender.hpp"

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
            auto& buf = active_buffer.value();

            sender.sendFrames(
                [&buf]() -> std::optional<image::Data>
                {
                    image::Data data;
                    buf.load();

                    if (false == buf.get(data))
                    {
                        return std::nullopt;
                    }
                    return data;
                });

            sender.run();
        }
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
    return 0;
}
