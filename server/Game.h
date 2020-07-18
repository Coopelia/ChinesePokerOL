#pragma once
#include"Def.h"
#include"PacketType.h"

class Game
{
public:
	Game();
	void start();
	void update();
	void game_logic();
	void setState(bool isPlaying);
	void setID(int id);
	void addPlayer(Customor*& c);
	void removePlayer(Customor*& c);
	int getNum();
	bool getState();//isPlaying?
	int getID();
	::std::vector<Customor*> getPlayersList();
	friend class Server;
private:
	int ID;//·¿¼äºÅ
	bool isPlaying;
	bool isDealing;
	bool isDealDizhu;
	::std::vector<Customor*> players;//Íæ¼Ò
	::std::vector<int> dizhuCard;
	::std::vector<int> tempCard;
};