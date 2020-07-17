#include "Connection.h"

Connection::Connection()
{
	ip = ::sf::IpAddress::getLocalAddress();
	port = 5678;
	timeout = ::sf::seconds(5);
	id = 0;
	isConnected = false;
}

Connection::~Connection()
{
	socket.disconnect();
}

bool Connection::connect()
{
	if (socket.connect(ip, port, timeout) != ::sf::Socket::Done)
	{
		isConnected = false;
		return false;
	}
	::sf::Packet p;
	socket.receive(p);
	p >> id;
	isConnected = true;
	return true;
}

void Connection::disconnect()
{
	socket.disconnect();
	isConnected = false;
}


int Connection::hostId()
{
	return this->id;
}

bool Connection::sendNetworkEvent(::pt::MSG_TYPE type, ::pt::NetworkEvent event)
{
	if (!isConnected)
		return false;
	::sf::Packet packet;
	packet <<id<< static_cast<int>(type) << event;

	mt_s.lock();
	q_sender.push(packet);
	mt_s.unlock();

	return true;
}

bool Connection::getNetworkEvent(::pt::NetworkEvent& nwe)
{
	::std::list<::pt::NetworkEvent>::iterator itr;
	mt_r.lock();
	for (itr = q_reciever.begin(); itr != q_reciever.end(); itr++)
	{
		if (itr->type() == nwe.type())
		{
			nwe = (*itr);
			q_reciever.erase(itr);
			mt_r.unlock();
			return true;
		}
	}
	mt_r.unlock();
	return false;
}

void Connection::receiveEvent()
{
	::sf::Packet packet;
	socket.receive(packet);
	int i;
	packet >> i;
	::pt::MSG_TYPE type = static_cast<::pt::MSG_TYPE>(i);
	::pt::NetworkEvent* msg = nullptr;
	switch (type)
	{
	case pt::reNull:
		break;
	case pt::reConnect:
		msg = new ::pt::ReConnect();
		break;
	case pt::reDisconnect:
		msg = new ::pt::ReDisconnect();
		break;
	case pt::reGetRoomList:
		msg = new ::pt::ReGetRoomList();
		break;
	case pt::reCreatRoom:
		msg = new ::pt::ReCreatRoom();
		break;
	case pt::reJoinRoom:
		msg = new ::pt::ReJoinRoom();
		break;
	case pt::reReady:
		msg = new ::pt::ReReady();
		break;
	case pt::reUnReady:
		msg = new ::pt::ReUnReady();
		break;
	case pt::reExitRoom:
		msg = new ::pt::ReExitRoom();
		break;
	case pt::reClearDeskCard:
		msg = new ::pt::ReClearDeskCard();
		break;
	case pt::daCallDec:
		msg = new ::pt::DaCallDec();
		break;
	case pt::daChuDec:
		msg = new ::pt::DaChuDec();
		break;
	case pt::daGameState:
		msg = new ::pt::DaGameState();
		break;
	case pt::daPlayerStateInfo_Ready:
		msg = new ::pt::DaPlayerStateInfo_Ready();
		break;
	case pt::daPlayerStateInfo_Call:
		msg = new ::pt::DaPlayerStateInfo_Call();
		break;
	case pt::daPlayerStateInfo_Chu:
		msg = new ::pt::DaPlayerStateInfo_Chu();
		break;
	case pt::daBeishu:
		msg = new ::pt::DaBeishu();
		break;
	case pt::daDizhuCard:
		msg = new ::pt::DaDizhuCard();
		break;
	case pt::daDeskCard:
		msg = new ::pt::DaDeskCard();
		break;
	case pt::daGameOver:
		msg = new ::pt::DaGameOver();
		break;
	case pt::daRoomList:
		msg = new ::pt::DaRoomList();
		break;
	case pt::daDealCard:
		msg = new ::pt::DaDealCard();
		break;
	default:
		break;
	}
	if (msg != nullptr)
	{
		mt_r.lock();
		q_reciever.push_back(*msg);
		mt_r.unlock();
	}
}

void Connection::sendEvent()
{
	mt_s.lock();
	socket.send(q_sender.front());
	q_sender.pop();
	mt_s.unlock();
}

Connection connector;