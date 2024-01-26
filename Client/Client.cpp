/*
** EPITECH PROJECT, 2024
** Babel
** File description:
** Client
*/

#include "Client.hpp"

Client::Client(boost::asio::io_context& io_context, const std::string& server_ip, unsigned short tcp_port) :
    _io_context(io_context),
    _tcp_socket(io_context),
    _udp_socket(io_context, udp::endpoint(udp::v4(), 0))
{
    // TCP connection
    tcp::resolver tcp_resolver(io_context);
    tcp::resolver::results_type tcp_endpoints = tcp_resolver.resolve(server_ip, std::to_string(tcp_port));
    boost::asio::connect(_tcp_socket, tcp_endpoints);
    readTcpMessage();
}

void Client::setUdpConnection(unsigned short udp_port)
{
    // UDP connection
    udp::resolver udp_resolver(_io_context);
    udp::resolver::results_type udp_endpoints = udp_resolver.resolve("127.0.0.1", std::to_string(udp_port));
    _udp_endpoint = *udp_endpoints.begin(); // Save the UDP endpoint for later use
    _udp_socket.connect(_udp_endpoint);
}

void Client::sendTcpMessage(const std::string& message) {
    std::cout << "Sending message: " << message << std::endl;
    boost::asio::write(_tcp_socket, boost::asio::buffer(message));
}

void Client::readTcpMessage() {
    boost::asio::streambuf buffer;
    boost::asio::read_until(_tcp_socket, buffer, "\n");
    std::string message = boost::asio::buffer_cast<const char*>(buffer.data());
    std::cout << "Message received: " << message << std::endl;

    if (message == "Hello\n")
        sendTcpMessage("Hello from Client to Server!\n");
    readTcpMessage();
}

void Client::sendUdpMessage(const std::string& message) {
    _udp_socket.send_to(boost::asio::buffer(message), _udp_endpoint);
}