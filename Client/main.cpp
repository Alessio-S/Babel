/*
** EPITECH PROJECT, 2024
** Babel
** File description:
** main
*/

#include <iostream>
#include <boost/asio.hpp>

#include "Client.hpp"

int main() {
    try {
        boost::asio::io_context io_context;
        bool first_message = true;

        Client client(io_context, "127.0.0.1", 8080);

        std::thread thread([&io_context](){ io_context.run(); });

        while (true) {
            std::string message;
            std::getline(std::cin, message, '\n');
            if (client.getUdpConnection() == true)
                client.sendUdpMessage(message);
            else {
                if (first_message == true) {
                    message = std::to_string(client.getUdpPort()) + "," + message;
                    first_message = false;
                }
                client.sendTcpMessage(message);
            }
        }
        client.getReadTcpThread().join();
        client.getReadUdpThread().join();
        thread.join();
    } catch (boost::system::system_error& e) {
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}