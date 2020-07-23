#pragma once
#include"MsgQueue.h"
#include"PacketType.h"

class Connection
{
public:
	Connection();
	~Connection();
	bool connect();
	void disconnect();
	int hostId();
	bool sendNetworkEvent(::sf::Packet packet);//调用这个函数发送信息
	bool getNetworkEvent(::pt::MSG_TYPE type, ::pt::NetworkEvent*& msg, bool fg);//调用这个函数获取信息(fg:true--遍历,false--get the front)
	void receiveEvent();//接收线程调用
	void sendEvent();//发送线程调用
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