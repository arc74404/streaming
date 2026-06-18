#include <boost/asio.hpp>

#include <iostream>

#include "../utils/global_params.hpp"

using boost::asio::ip::tcp;

int
main()
{
    try
    {
        boost::asio::io_context context;
        tcp::acceptor acceptor(context, tcp::endpoint(tcp::v4(), 9999));

        while (true)
        {
            tcp::socket socket(context);

            acceptor.accept(socket);

            std::cout << "Connection!" << std::endl;

            try
            {
                char data[1024];
                boost::system::error_code error;

                size_t length =
                    socket.read_some(boost::asio::buffer(data), error);

                if (error == boost::asio::error::eof)
                {
                    std::cout << "Disconnect" << std::endl;
                    continue;
                }
                else if (error)
                {
                    throw boost::system::system_error(error);
                }
                socket.close();
            }
            catch (std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
