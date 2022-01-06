#include"Customor.h"

Customor::Customor()
{
	isConnected = true;
	id = 0;
}

Customor::~Customor()
{

}

void Customor::connect(::sf::IpAddress ip, int port, int id)
{
	this->id = id;
	this->ip = socket_in.getRemoteAddress();
	this->port = port;
	socket_out.connect(ip, port);
	isConnected = true;
	::sf::Packet p;
	p << id;
	socket_out.send(p);
	::std::cout << "客户端已连接，ID：" << id << "\nIP：" << ip << "\n";
}

void Customor::disconnect()
{
	::sf::Packet p;
	int i = 886;
	p << i;
	socket_out.send(p);
	socket_in.disconnect();
	socket_out.disconnect();
	isConnected = false;
	::std::cout << "客户端已断开连接，ID：" << id << "\nIP：" << ip << "\n";
}

::sf::TcpSocket& Customor::getSocketIn()
{
	return socket_in;
}

::sf::TcpSocket& Customor::getSocketOut()
{
	return socket_out;
}

int Customor::Id()
{
	return id;
}

void Customor::sendNetworkEvent(::sf::Packet packet)
{
	mt_s.lock();
	q_sender.push(packet);
	mt_s.unlock();
}

bool Customor::getNetworkEvent(::pt::MSG_TYPE type, ::pt::NetworkEvent*& msg)
{
	if (q_reciever.empty())
		return false;
	::sf::Packet packet;
	int i;
	mt_r.lock();
	packet = q_reciever.front();
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
		q_reciever.pop();
	}

	mt_r.unlock();
	if (msg == nullptr)
		return false;
	return true;
}

::pt::NetworkEvent* Customor::proccessEvent()
{
	::pt::NetworkEvent* msg = nullptr;
	mt_r.lock();
	if (!q_reciever.empty())
	{
		::sf::Packet packet = q_reciever.front();
		int i;
		packet >> i;
		::pt::MSG_TYPE type = static_cast<::pt::MSG_TYPE>(i);
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
		q_reciever.pop();
	}
	mt_r.unlock();
	return msg;
}

void Customor::pth_update(Customor* _this)
{
	while (true)
	{
		if (!_this->isConnected || _this == NULL)
		{
			_this->disconnect();
			return;
		}
	}
}

void Customor::pth_send(Customor* _this)
{
	while (_this->isConnected && _this != NULL)
	{
		_this->mt_s.lock();
		if (!_this->q_sender.empty())
		{
			_this->socket_out.send(_this->q_sender.front());
			_this->q_sender.pop();
		}
		_this->mt_s.unlock();
	}
}

void Customor::pth_recieve(Customor* _this)
{
	/*::sf::SocketSelector socket_selector;
	socket_selector.add(_this->socket_in);*/
	while (_this->isConnected && _this != NULL)
	{
		/*if (not socket_selector.wait(sf::seconds(1)))
			continue;
		if (not socket_selector.isReady(_this->socket_in))
			continue;*/
		::sf::Packet packet;
		if(_this->socket_in.receive(packet)==::sf::Socket::Done)
			::std::cout << "recieved a message\n";
		::sf::Packet pt = packet;
		int i;
		pt >> i;
	/*	if (i == 10)
			i = 10;*/
		if (i == 886)
		{
			_this->isConnected = false;
			return;
		}
		_this->mt_r.lock();
		_this->q_reciever.push(packet);
		_this->mt_r.unlock();
	}
}

void Customor::runThreads(Customor* _this)
{
	::std::thread th1 = ::std::thread(Customor::pth_send, _this);
	::std::thread th2 = ::std::thread(Customor::pth_recieve, _this);
	::std::thread th3 = ::std::thread(Customor::pth_update, _this);
	th1.detach();
	th2.detach();
	th3.detach();
}