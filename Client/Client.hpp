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

        unsigned short getUdpPort() { return _udp_socket.local_endpoint().port(); } 

        std::atomic<bool>& getUdpConnection() { return _udp_connection; }
        std::thread& getReadTcpThread() { return _read_tcp_thread; }
        std::thread& getAudioThread() { return _audio_thread; }
        std::thread& getReadUdpThread() { return _read_udp_thread; }

        void setUdpPort(unsigned short udp_port) { _udp_port = udp_port; }
        void setUdpConnection(unsigned short udp_port);

        void sendTcpMessage(const std::string& message);
        void sendUdpMessage(const std::string& message);

        void readTcpMessage();
        void readUdpMessage();

        void startCall();
        void sendAudioStream(std::vector<float> audioStream);
        std::vector<float> readAudioStream();

    protected:
    private:
        boost::asio::io_context& _io_context;
        std::atomic<bool> _udp_connection = false;
        tcp::socket _tcp_socket;
        unsigned short _udp_port;
        udp::socket _udp_socket;
        udp::endpoint _udp_endpoint;

        std::thread _audio_thread;
        std::thread _read_tcp_thread;
        std::thread _read_udp_thread;
};

#endif /* !CLIENT_HPP_ */