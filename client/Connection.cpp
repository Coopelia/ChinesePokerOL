#include "Connection.h"

Connection::Connection()
{
	host_ip = ::sf::IpAddress::getLocalAddress();
	port_self = 3456;
	port_host = 5678;
	timeout = ::sf::seconds(5);
	id = 0;
	isConnected = false;
}

Connection::~Connection()
{
	disconnect();
}

bool Connection::connect()
{
	if (socket_out.connect(host_ip, port_host, timeout) != ::sf::Socket::Done)
	{
		isConnected = false;
		return false;
	}
	::sf::Packet p;
	p << port_self;
	socket_out.send(p);
	p.clear();
	listener.listen(port_self);
	if (listener.accept(socket_in) == ::sf::Socket::Done)
	{
		::std::cout << "host ip:" << socket_in.getRemoteAddress() << "\n";
		socket_in.receive(p);
		p >> id;
		::std::cout << "my id is: " << id << "\nall is good\n";
		isConnected = true;
	}
	else
		return false;

	return true;
}

void Connection::disconnect()
{
	::sf::Packet p;
	int i = 886;
	p << i;
	socket_out.send(p);
	socket_in.disconnect();
	socket_out.disconnect();
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
	packet << static_cast<int>(type) << event;

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
	socket_in.receive(packet);
	int i;
	packet >> i;
	if (i == 886)
	{
		isConnected = false;
		return;
	}
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
		msg = new ::pt::ReJoinRoom(0);
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
		packet >> *msg;
		q_reciever.push_back(*msg);
		mt_r.unlock();
	}
}

void Connection::sendEvent()
{
	mt_s.lock();
	socket_out.send(q_sender.front());
	q_sender.pop();
	mt_s.unlock();
}

Connection connector;