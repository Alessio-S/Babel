#include "Session.hpp"
#include "ServerTcp.hpp"

Session::Session(boost::asio::io_context& io_context)
    : _socket(io_context) {}

Session::pointer Session::create(boost::asio::io_context& io_context)
{
    return pointer(new Session(io_context));
}

Session::~Session()
{
    _socket.close();
}

void Session::setClientData(std::string username)
{
    _client.username = username;
	_client.code = std::to_string(std::rand() % 1000 + 4000);
	_client.port = _socket.remote_endpoint().port();
    _client.connected = true;
}

std::string Session::serializeClientData()
{
    std::string json = "{\n";
    json += "\t\"username\": \"" + _client.username + "\",\n";
    json += "\t\"code\": \"" + _client.code + "\",\n";
    json += "\t\"port\": \"" + std::to_string(_client.port) + "\"\n";
    json += "\t\"connected\": \"" + std::to_string(_client.connected) + "\"\n";
    json += "}";
    return json;
}

void Session::start()
{
    std::cout << "Session started" << std::endl;
    // Send a welcome message to the connected user
    std::string message = "Welcome to the server!\nPlase Enter your username\n";
    // From here the server crashes
    boost::asio::async_write(_socket, boost::asio::buffer(message),
                            std::bind(&Session::handleWrite, shared_from_this(),
                            std::placeholders::_1, std::placeholders::_2));
    
    // Schedule reading data from the client
    _socket.async_read_some(boost::asio::buffer(_data, max_length),
                            std::bind(&Session::handleRead, shared_from_this(),
                            std::placeholders::_1, std::placeholders::_2));
}

void Session::handleRead(const boost::system::error_code& error, size_t bytes_transferred) {
    if (!error) {
        // Process the received data and decide what to do
        std::string received_data(_data, bytes_transferred);
        received_data = regularizeResponse(received_data);
        
        if (!_client.connected) {
            setClientData(received_data);
            ServerTcp::GetInstance()->addClient(_client);
            handleRead(error, bytes_transferred);
        } else {
            // std::cout << "Received data: " << received_data << std::endl;
            // // Write the response back to the client
            // std::string response = _client.username + ": " + received_data + "\n";
            // writeResponse(response);
            printClients();
        }
    } else if (error == boost::asio::error::eof) {
        std::cout << "Client disconnected" << std::endl;
        _client.connected = false;
        _socket.cancel();
        // _socket.close();
    } else {
        std::cerr << "Error reading data: " << error.message() << std::endl;
    }
}

void Session::writeResponse(const std::string& response) {
    // Write the response back to the client
    boost::asio::async_write(_socket, boost::asio::buffer(response),
        std::bind(&Session::handleWrite, shared_from_this(),
            std::placeholders::_1, std::placeholders::_2));
}

std::string Session::regularizeResponse(const std::string &response)
{
    std::string regularized_response = response;
    if (regularized_response.back() == '\n') {
        regularized_response.pop_back();
    }
    return regularized_response;
}

void Session::printClients()
{
    std::string response = "Clients: \n";
    for (auto &client : ServerTcp::GetInstance()->getClients()) {
        response += "\t" + client.username + "#" + client.code + "(" + std::to_string(client.port) + ")";
        if (client.connected)
            response += " - Connected\n";
        else
            response += " - Disconnected\n";
    }
    writeResponse(response);
}

void Session::handleWrite(const boost::system::error_code& error, size_t bytes_transferred) {
    if (!error) {
        // Continue reading data from the client
        _socket.async_read_some(boost::asio::buffer(_data, max_length),
            std::bind(&Session::handleRead, shared_from_this(),
                std::placeholders::_1, std::placeholders::_2));
    } else {
        std::cerr << "Error writing data: " << error.message() << std::endl;
    }
}
