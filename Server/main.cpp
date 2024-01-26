/*
** EPITECH PROJECT, 2024
** Babel
** File description:
** main
*/

#include <iostream>
#include <boost/asio.hpp>

#include "ServerTcp.hpp"

int main() {
    try {
        boost::asio::io_context io_context;

        // Start the TCP server
        ServerTcp server(io_context, 8080);

        io_context.run();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
