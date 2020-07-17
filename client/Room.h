#pragma once
#include<vector>

class Room
{
public:
	Room();
	void setState(bool isPlaying);
	void setID(int id);
	void setNum(int num);
	void addPlayer(int id);
	void removePlayer(int id);
	int getNum();
	bool getState();//isPlaying?
	int getID();
	::std::vector<int> getPlayersId();
private:
	int ID;//房间号
	bool isPlaying;
	int num_player;//房间当前玩家数量
	::std::vector<int> playersId;//玩家id
};