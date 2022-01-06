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
	void sendNetworkEvent(::sf::Packet packet);//�����������������Ϣ
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

	::std::queue<::sf::Packet> q_sender;//��������Ϣ����
	::sf::Mutex mt_s;//���ʸö��еĻ�����

	::std::queue<::sf::Packet> q_reciever;//��������Ϣ����
	::sf::Mutex mt_r;//���ʸö��еĻ�����
};