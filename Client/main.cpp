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
    std::cout << "Hello, World!" << std::endl;

    boost::asio::io_context io_context;

    Client client(io_context, "127.0.0.1", 8080);

    // Creating the second client and connecting to the server
    // Client client2(io_context, "127.0.0.1", 8080);

    // Setting up UDP connection between clients
    // client1.setUdpConnection("127.0.0.1", 8083);
    // client2.setUdpConnection("127.0.0.1", 8083);

    // Sending messages over TCP and UDP between clients
    // client.sendTcpMessage("Hello from Client to Server!");
    // client2.sendTcpMessage("Hello from Client2 to Server!");

    // Uncomment the lines below to enable UDP messaging between clients
    // client1.sendUdpMessage("Hello UDP from Client1 to Client2!");
    // client2.sendUdpMessage("Hello UDP from Client2 to Client1!");

    io_context.run();
    return 0;
}
