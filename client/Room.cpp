#include"Room.h"

Room::Room()
{
	ID = -1;
	isPlaying = false;
	num_player = 0;
}

void Room::setState(bool isPlaying)
{
	this->isPlaying = isPlaying;
}

void Room::setID(int id)
{
	this->ID = id;
}

void Room::setNum(int num)
{
	this->num_player = num;
}

void Room::addPlayer(int id)
{
	this->playersId.push_back(id);
	this->num_player++;
}

void Room::removePlayer(int id)
{
	::std::vector<int>::iterator itr;
	for( itr=playersId.begin();itr!=playersId.end();)
	{
		if (*itr == id)
		{
			playersId.erase(itr);
			return;
		}
		itr++;
	}
	this->num_player--;
}

int Room::getNum()
{
	return this->num_player;
}

bool Room::getState()
{
	return this->isPlaying;
}

int Room::getID()
{
	return this->ID;
}

::std::vector<int> Room::getPlayersId()
{
	return this->playersId;
}