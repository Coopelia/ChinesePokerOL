#pragma once
#include"MsgQueue.h"
#include"PacketType.hpp"

class Connection
{
public:
	Connection();
	~Connection();
	bool connect();
	void disconnect();
	int hostId();
	bool sendNetworkEvent(::pt::MSG_TYPE type, ::pt::NetworkEvent event);//调用这个函数发送信息
	bool getNetworkEvent(::pt::NetworkEvent& nwe);//调用这个函数获取信息
	void receiveEvent();//接收线程调用
	void sendEvent();//发送线程调用
	bool isConnected;
private:
	::sf::TcpSocket socket;
	::sf::IpAddress ip;
	::sf::Time timeout;
	uint32_t port;
	int id;
};

extern Connection connector;