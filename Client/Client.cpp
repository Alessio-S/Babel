/*
** EPITECH PROJECT, 2024
** Babel
** File description:
** Client
*/

#include "Client.hpp"

#include "AudioStream.hpp"

Client::Client(boost::asio::io_context& io_context, const std::string& server_ip, unsigned short tcp_port) :
    _io_context(io_context),
    _tcp_socket(io_context),
    _udp_socket(io_context, udp::endpoint(udp::v4(), 0))
{
    // TCP connection
    tcp::resolver tcp_resolver(io_context);
    tcp::resolver::results_type tcp_endpoints = tcp_resolver.resolve(server_ip, std::to_string(tcp_port));
    boost::asio::connect(_tcp_socket, tcp_endpoints);

    _read_tcp_thread = std::thread([this] { readTcpMessage(); });
}

void Client::setUdpConnection(unsigned short udp_port)
{
    udp::resolver udp_resolver(_io_context);
    udp::resolver::results_type udp_endpoints = udp_resolver.resolve("127.0.0.1", std::to_string(udp_port));
    _udp_endpoint = *udp_endpoints.begin(); // Save the UDP endpoint for later use
    _udp_socket.connect(_udp_endpoint);

    _udp_connection = true;
    _read_udp_thread = std::thread([this] { readUdpMessage(); });
    sendUdpMessage("CALL\n");
}

void Client::sendTcpMessage(const std::string& message) {
    boost::asio::write(_tcp_socket, boost::asio::buffer(message));
}

void Client::readTcpMessage() {
    boost::asio::streambuf buffer;
    boost::asio::read_until(_tcp_socket, buffer, "\n");
    std::string message = boost::asio::buffer_cast<const char*>(buffer.data());

    if (message == "Hello\n")
        sendTcpMessage("Hello from Client to Server!\n");
    else if (message.find("UDP_PORT:") == 0) {
        std::string udp_port = message.substr(9, message.size() - 10);
        setUdpConnection(std::stoi(udp_port));
    } else {
        std::cout << message << std::endl;
    }
    readTcpMessage();
}

void Client::readUdpMessage()
{
    while (true) {
        char data[1024];
        size_t length = _udp_socket.receive_from(boost::asio::buffer(data), _udp_endpoint);
        std::string message(data, length);

        if (message == "CALL\n")
            startCall();
        else 
            std::cout << message << std::endl;
    }
}

void Client::startCall()
{
    std::cout << "Call started" << std::endl;
    AudioStream audioStream([this](AudioStream *stream, const float *inputBuffer, float *outputBuffer, unsigned long frameCount) {
        std::vector<float> inputStream;
        inputStream.reserve(frameCount);
        inputStream.insert(inputStream.end(), inputBuffer, inputBuffer + frameCount);
        sendAudioStream(inputStream);
        std::vector<float> audioStream = readAudioStream();
        std::copy(audioStream.begin(), audioStream.end(), outputBuffer);
    });
    audioStream.recordMicrophone();
    while (true) {
        Pa_Sleep(10000);
    }
}

void Client::sendAudioStream(std::vector<float> audioStream)
{
    _udp_socket.send_to(boost::asio::buffer(audioStream), _udp_endpoint);
}

std::vector<float> Client::readAudioStream()
{
    std::vector<float> audioStream;
    _udp_socket.receive_from(boost::asio::buffer(audioStream), _udp_endpoint);
    return audioStream;
}

void Client::sendUdpMessage(const std::string& message) {
    _udp_socket.send_to(boost::asio::buffer(message), _udp_endpoint);
}

