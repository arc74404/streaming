#include <boost/asio.hpp>

#include <iostream>

#include "server.hpp"

#include "../utils/global_params.hpp"

int
main()
{
    try
    {
        boost::asio::io_context context;

        stream::server::Server server(context);

        context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
