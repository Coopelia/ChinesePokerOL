#include "Connection.h"

Connection::Connection()
{
	ip = ::sf::IpAddress::getLocalAddress();
	port = 5678;
	timeout = ::sf::seconds(5);
}

bool Connection::connect()
{
	return socket.connect(ip, port, timeout) == ::sf::Socket::Done ? true : false;
}

void Connection::disconnect()
{
	socket.disconnect();
}

bool Connection::send(::pt::MSG_TYPE type, ::pt::NetworkEvent event)
{
	::sf::Packet packet;
	packet << static_cast<int>(type) << event;
	socket.send(packet);
}

::pt::NetworkEvent Connection::receive(::sf::Packet packet)
{
	int i;
	packet >> i;
	::pt::MSG_TYPE type = static_cast<::pt::MSG_TYPE>(i);
	switch (type)
	{
	case pt::IdConnect_handler:
		break;
	case pt::IdConnected:
		break;
	case pt::IdDisconnected:
		break;
	case pt::IdLogOut:
		break;
	case pt::IdGetListGame:
		break;
	case pt::IdSetListGame:
		break;
	case pt::IdCreateGame:
		break;
	case pt::IdJoinGame:
		break;
	case pt::IdJoinGameOk:
		break;
	case pt::IdJoinGameReject:
		break;
	case pt::IdUpdateSF:
		break;
	case pt::IdUpdateDeskCard:
		break;
	case pt::IdUpdateHandCard:
		break;
	case pt::IdUpdateState:
		break;
	case pt::IdUpdateTimeChuPai:
		break;
	case pt::IdUpdateBeiShu:
		break;
	case pt::IdUpdateGameOver:
		break;
	case pt::IdUpdateTimeCall:
		break;
	case pt::IdUpdateScoreCall:
		break;
	case pt::IdUpdateResultCall:
		break;
	default:
		break;
	}
}