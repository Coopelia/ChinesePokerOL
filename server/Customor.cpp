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

void ::Customor::disconnect()
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

void Customor::sendNetworkEvent(::pt::MSG_TYPE type, ::pt::NetworkEvent event)
{
	if (!isConnected)
		return;
	::sf::Packet packet;
	packet << static_cast<int>(type) << event;

	mt_s.lock();
	q_sender.push(packet);
	mt_s.unlock();
}

bool Customor::getNetworkEvent(::pt::NetworkEvent& nwe)
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

::pt::NetworkEvent* Customor::proccessEvent()
{
	::pt::NetworkEvent e(::pt::reNull);
	mt_r.lock();
	if (!q_reciever.empty())
	{
		e = q_reciever.front();
		q_reciever.pop_front();
	}
	mt_r.unlock();
	return &e;
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
	::sf::SocketSelector socket_selector;
	socket_selector.add(_this->socket_in);
	while (_this->isConnected && _this != NULL)
	{
		if (not socket_selector.wait(sf::seconds(1)))
			continue;
		if (not socket_selector.isReady(_this->socket_in))
			continue;
		::sf::Packet packet;
		if(_this->socket_in.receive(packet)==::sf::Socket::Done)
			::std::cout << "recieved a message\n";
		int i;
		packet >> i;
		if (i == 886)
		{
			_this->isConnected = false;
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
			_this->mt_r.lock();
			packet >> *msg;
			_this->q_reciever.push_back(*msg);
			_this->mt_r.unlock();
		}
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