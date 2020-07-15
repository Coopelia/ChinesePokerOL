#pragma once
#include<SFML/Network.hpp>
#include"PacketType.hpp"

class Connection
{
public:
	Connection();
	~Connection();
	bool connect();
	void disconnect();
	bool send(::pt::MSG_TYPE type, ::pt::NetworkEvent event);
	::pt::NetworkEvent receive(::sf::Packet packet);
private:
	::sf::TcpSocket socket;
	::sf::IpAddress ip;
	::sf::Time timeout;
	uint32_t port;
};