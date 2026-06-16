#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

#include "image_data/capturer.hpp"
#include "io/sender.hpp"
#include "utils/write.hpp"

#include "global_params.hpp"

using namespace stream;

int
main()
{
    std::cout << "Start\n";
    GlobalParams<int, std::string> global_params;
    global_params.addParam<int>("first", 4);
    global_params.addParam<std::string>("second", "cat");

    int first = global_params.getParam<int>("first");

    std::cout << first << '\n';

    std::cout << global_params.getParam<std::string>("second");

    // std::setlocale(LC_ALL, "");
    // image::Capturer capturer;

    // if (false == capturer.prepare())
    // {
    //     return 1;
    // }

    // auto&& info = capturer.screensInfo();

    // auto active_buffer = capturer.capture(0);

    // io::Sender sender;

    // if (active_buffer.has_value())
    // {
    //     image::Data data;

    //     auto& buf = active_buffer.value();

    //     int iter_count = 0;

    //     while (iter_count < 5)
    //     {
    //         buf.load();

    //         if (false == buf.get(data))
    //         {
    //             continue;
    //         }

    //         // splitOnChunks();

    //         ++iter_count;

    //         sender.sendToServer(data);

    //         // utils::writeInFile(data,
    //         //                 "screen_" + std::to_string(iter_count) +
    //         ".png");
    //     }
    // }
    return 0;
}
