#pragma once
#include"Def.h"
#include"Connection.h"
#include"Game.h"

class Server
{
public:
	Server();
	~Server();
	void Start();
	void Update();
	void Run();
private:
	Connection connector;
	::std::list<Game*> games;
	void update_games();
};