#include"Room.h"

void Room::setRest(int rest)
{
	this->rest = rest;
}

void Room::setState(bool isPlaying)
{
	this->isPlaying = isPlaying;
}

void Room::setID(int id)
{
	this->ID = id;
}

int Room::getRest()
{
	return this->rest;
}

int Room::getState()
{
	return this->isPlaying;
}

int Room::getID()
{
	return this->ID;
}