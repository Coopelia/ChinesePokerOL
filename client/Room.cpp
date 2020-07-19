#include"Room.h"

Room::Room()
{
	ID = -1;
	isPlaying = false;
}

void Room::setState(bool isPlaying)
{
	this->isPlaying = isPlaying;
}

void Room::setID(int id)
{
	this->ID = id;
}

void Room::addPlayer(int id)
{
	this->playersId.push_back(id);
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
}

int Room::getNum()
{
	return this->playersId.size();
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