/*
** EPITECH PROJECT, 2024
** Babel
** File description:
** Client
*/

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

class Client {
    public:
        Client(boost::asio::io_context& io_context, const std::string& server_ip, unsigned short tcp_port);
        ~Client() {};

        void setUdpPort(unsigned short udp_port) { _udp_port = udp_port; }
        void setUdpConnection(unsigned short udp_port);

        void sendTcpMessage(const std::string& message);
        void sendUdpMessage(const std::string& message);

        void readTcpMessage();

    protected:
    private:
        boost::asio::io_context& _io_context;
        tcp::socket _tcp_socket;
        unsigned short _udp_port;
        udp::socket _udp_socket;
        udp::endpoint _udp_endpoint;
};

#endif /* !CLIENT_HPP_ */
