#include "Connection.h"

Connection::Connection()
{
	//host_ip = ::sf::IpAddress::getLocalAddress();
	host_ip = "60.176.45.108";
	port_self = 5688;
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
	int retry = 10;
	while (listener.listen(port_self) != ::sf::Socket::Status::Done&&retry>0)
	{
		::std::cout << "端口" << port_self++ << "已被占用\n";
		retry--;
	}
	if (retry < 0)
	{
		::std::cout << "寻找端口时间超过限制，将无法接收来自服务器的请求\n";
		return false;
	}
	else
		::std::cout << "已绑定侦听端口" << port_self << "\n";
	::sf::Packet p;
	p << port_self;
	socket_out.send(p);
	p.clear();
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

bool Connection::sendNetworkEvent(::sf::Packet packet)
{
	if (!isConnected)
		return false;
	mt_s.lock();
	q_sender.push(packet);
	mt_s.unlock();

	return true;
}

bool Connection::getNetworkEvent(::pt::MSG_TYPE type, ::pt::NetworkEvent*& msg)
{
	::std::list<::sf::Packet>::iterator itr;
	::sf::Packet packet;
	int i;
	mt_r.lock();
	for (itr = q_reciever.begin(); itr != q_reciever.end(); itr++)
	{
		packet = (*itr);
		packet >> i;
		::pt::MSG_TYPE _type = static_cast<::pt::MSG_TYPE>(i);
		if (_type == type)
		{
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
				packet >> (*static_cast<::pt::ReJoinRoom*>(msg));
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
				packet >> (*static_cast<::pt::DaCallDec*>(msg));
				break;
			case pt::daChuDec:
				msg = new ::pt::DaChuDec();
				packet >> (*static_cast<::pt::DaChuDec*>(msg));
				break;
			case pt::daGameState:
				msg = new ::pt::DaGameState();
				packet >> (*static_cast<::pt::DaGameState*>(msg));
				break;
			case pt::daPlayerStateInfo_Ready:
				msg = new ::pt::DaPlayerStateInfo_Ready();
				packet >> (*static_cast<::pt::DaPlayerStateInfo_Ready*>(msg));
				break;
			case pt::daPlayerStateInfo_Call:
				msg = new ::pt::DaPlayerStateInfo_Call();
				packet >> (*static_cast<::pt::DaPlayerStateInfo_Call*>(msg));
				break;
			case pt::daPlayerStateInfo_Chu:
				msg = new ::pt::DaPlayerStateInfo_Chu();
				packet >> (*static_cast<::pt::DaPlayerStateInfo_Chu*>(msg));
				break;
			case pt::daBeishu:
				msg = new ::pt::DaBeishu();
				packet >> (*static_cast<::pt::DaBeishu*>(msg));
				break;
			case pt::daDizhuCard:
				msg = new ::pt::DaDizhuCard();
				packet >> (*static_cast<::pt::DaDizhuCard*>(msg));
				break;
			case pt::daDeskCard:
				msg = new ::pt::DaDeskCard();
				packet >> (*static_cast<::pt::DaDeskCard*>(msg));
				break;
			case pt::daGameOver:
				msg = new ::pt::DaGameOver();
				packet >> (*static_cast<::pt::DaGameOver*>(msg));
				break;
			case pt::daRoomList:
				msg = new ::pt::DaRoomList();
				packet >> (*static_cast<::pt::DaRoomList*>(msg));
				break;
			case pt::daDealCard:
				msg = new ::pt::DaDealCard();
				packet >> (*static_cast<::pt::DaDealCard*>(msg));
				break;
			default:
				break;
			}
			q_reciever.erase(itr);
			break;
		}
	}
	mt_r.unlock();
	if (msg == nullptr)
		return false;
	return true;
}

void Connection::receiveEvent()
{
	::sf::Packet packet;
	socket_in.receive(packet);
	mt_r.lock();
	q_reciever.push_back(packet);
	mt_r.unlock();
}

void Connection::sendEvent()
{
	mt_s.lock();
	socket_out.send(q_sender.front());
	q_sender.pop();
	mt_s.unlock();
}

Connection connector;