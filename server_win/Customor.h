#pragma once
#include"Def.h"
#include"PacketType.h"
#include"Player.h"

class Customor
{
public:
	Customor();
	~Customor();
	void connect(::sf::IpAddress ip, int port, int id);
	void disconnect();
	int Id();
	::sf::TcpSocket& getSocketIn();
	::sf::TcpSocket& getSocketOut();
	void sendNetworkEvent(::sf::Packet packet);//调用这个函数发送信息
	bool getNetworkEvent(::pt::MSG_TYPE type, ::pt::NetworkEvent*& msg);
	::pt::NetworkEvent* proccessEvent();
	static void pth_send(Customor* _this);
	static void pth_recieve(Customor* _this);
	static void pth_update(Customor* _this);
	static void runThreads(Customor* _this);
	bool isConnected;
	Player player;
private:
	int id;
	int port;

	::sf::IpAddress ip;
	::sf::TcpSocket socket_in;
	::sf::TcpSocket socket_out;

	::std::queue<::sf::Packet> q_sender;//发送者消息队列
	::sf::Mutex mt_s;//访问该队列的互斥锁

	::std::queue<::sf::Packet> q_reciever;//接收者消息队列
	::sf::Mutex mt_r;//访问该队列的互斥锁
};