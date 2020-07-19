#pragma once
#include"Def.h"
#include"Room.h"
#include"Button.h"
#include"Connection.h"
#include"MyClock.h"

class RoomGUI
{
public:
	RoomGUI();
	void initial_window(::sf::RenderWindow* app);
	void setInfo(::std::vector<Room>& room);
	void show();
	void update_room_list();
	int onClick(::sf::Event& e);
	friend class StartScene;
private:
	::sf::RenderWindow* app;
	::sf::Sprite sBackMenu, sBackSelet;
	::sf::Texture tBackMenu, tBackSelet;
	::sf::Text text;
	::sf::Font font;
	::std::vector<Button> button;
	::std::vector<Room> room;
};