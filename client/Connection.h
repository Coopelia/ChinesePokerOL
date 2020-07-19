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
	bool sendNetworkEvent(::sf::Packet packet);//�����������������Ϣ
	bool getNetworkEvent(::pt::MSG_TYPE type, ::pt::NetworkEvent*& msg);//�������������ȡ��Ϣ
	void receiveEvent();//�����̵߳���
	void sendEvent();//�����̵߳���
	bool isConnected;
private:
	::sf::TcpListener listener;
	::sf::TcpSocket socket_in;
	::sf::TcpSocket socket_out;
	::sf::IpAddress host_ip;
	::sf::Time timeout;
	uint32_t port_host;
	uint32_t port_self;
	int id;
};

extern Connection connector;