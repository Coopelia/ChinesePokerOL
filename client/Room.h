#pragma once
#include<vector>

class Room
{
public:
	Room();
	void setState(bool isPlaying);
	void setID(int id);
	void addPlayer(int id);
	void removePlayer(int id);
	int getNum();
	bool getState();//isPlaying?
	int getID();
	::std::vector<int> getPlayersId();
private:
	int ID;//·¿¼äºÅ
	bool isPlaying;
	::std::vector<int> playersId;//Íæ¼Òid
};