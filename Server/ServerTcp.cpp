/*
** EPITECH PROJECT, 2024
** Babel
** File description:
** ServerTcp
*/

#include "ServerTcp.hpp"

ServerTcp *ServerTcp::_instance = nullptr;

ServerTcp::ServerTcp(boost::asio::io_context& io_context, unsigned short port) :
    _io_context(io_context),
    _acceptor(io_context, tcp::endpoint(tcp::v4(), port)),
    _socket(io_context) {

	_instance = this;
	startAccept();
}

void ServerTcp::addClient(ClientData client)
{
	_clients.emplace_back(client);
	std::cout << "Client added" << std::endl;
}

void ServerTcp::removeClient(ClientData client)
{
	for (auto it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->port == client.port) {
			_clients.erase(it);
			std::cout << "Client removed" << std::endl;
			return;
		}
	}
}

unsigned short ServerTcp::getAvailableClient(unsigned short port)
{
	for (auto it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->port == port)
			return it->port;
	}
	std::cout << "No available client on port: " << port << std::endl;
	return -1;
}

void ServerTcp::startAccept()
{
    Session::pointer newSession = Session::create(_io_context);
	_acceptor.async_accept(newSession->getSocket(),
		boost::bind(&ServerTcp::handleAccept, this, newSession,
		boost::asio::placeholders::error));
}

void ServerTcp::handleAccept(Session::pointer session, const boost::system::error_code& error)
{
	if (!error) {
		session->start();
		startAccept();
	}
}
