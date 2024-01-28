/*
** EPITECH PROJECT, 2024
** Babel
** File description:
** serverTcp
*/

#ifndef SERVERTCP_HPP_
#define SERVERTCP_HPP_

#include <vector>
#include <random>

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "Session.hpp"

using boost::asio::ip::tcp;

class ServerTcp {
    public:
        ServerTcp(boost::asio::io_context& io_context, unsigned short port);
        static ServerTcp* GetInstance() { return _instance; }
        ~ServerTcp() {};

        std::vector<ClientData> &getClients() { return _clients; };
        void addClient(ClientData client);
        void removeClient(ClientData client);
        unsigned short getAvailableClient(unsigned short port);

    private:
        static ServerTcp* _instance;
        void startAccept();
        void handleAccept(Session::pointer session, const boost::system::error_code& error);

        boost::asio::io_context& _io_context;
        tcp::acceptor _acceptor;
        tcp::socket _socket;

        std::vector<ClientData> _clients;
};

#endif /* !SERVERTCP_HPP_ */
