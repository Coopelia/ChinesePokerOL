#pragma once
#include"Def.h"

class Connection
{
public:
	Connection();
	void Stop();
	static void pth_recieve_new_cust(Connection* _this);
	static void pth_update(Connection* _this);
	static void runThreads(Connection* _this);
private:
	uint32_t port_self;
	::sf::TcpListener listener;
	int clientIdTurn;
	bool stop;
	void recieve_new_cust();
};

//extern Connection connector;