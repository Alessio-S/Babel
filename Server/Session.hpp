/*
** EPITECH PROJECT, 2024
** Babel
** File description:
** Session
*/

#ifndef SESSION_HPP_
#define SESSION_HPP_

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/enable_shared_from_this.hpp>

using boost::asio::ip::tcp;

struct ClientData {
    std::string username = "User";
    std::string code = "3000";
    unsigned short port;
    int connected = false;
};

class Session : public boost::enable_shared_from_this<Session> {
    public:
        typedef boost::shared_ptr<Session> pointer;
        static pointer create(boost::asio::io_context& io_context);
        ~Session();

        tcp::socket& getSocket() { return _socket; }
        void setClientData(std::string username);
        std::string serializeClientData();

        void start();
        void handleRead(const boost::system::error_code& error, size_t bytes_transferred);
        void handleWrite(const boost::system::error_code& error, size_t bytes_transferred);

        void writeResponse(const std::string& response);
        std::string regularizeResponse(const std::string& response);
        void printClients();

    protected:
    private:
        Session(boost::asio::io_context& io_context);
        tcp::socket _socket;
        enum { max_length = 1024 };
        char _data[max_length];
        ClientData _client = ClientData();
};

#endif /* !SESSION_HPP_ */
