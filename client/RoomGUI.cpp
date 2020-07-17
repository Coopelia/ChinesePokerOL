#include"RoomGUI.h"

RoomGUI::RoomGUI()
{
	this->font.loadFromFile("assets/fonts/fSimpleRound.ttf");
	this->text.setFont(font);
	this->text.setCharacterSize(24);
	this->text.setFillColor(::sf::Color::Black);
	this->tBackMenu.loadFromFile("assets/image/game/���/������Ϸ���汳��.png");
	this->sBackMenu.setTexture(tBackMenu);
	this->tBackSelet.loadFromFile("assets/image/game/���/��������/room_sel.png");
	this->sBackSelet.setTexture(tBackSelet);
}

void RoomGUI::initial_window(::sf::RenderWindow* app)
{
	this->app = app;
}

void RoomGUI::setInfo(::std::vector<Room>& room)
{
	this->room = room;
	this->button.resize(room.size());
	for (int i = 0; i < room.size(); i++)
		button[i].setTextrue("assets/image/game/��������/#��ť/bt_join.png");
}

void RoomGUI::update()
{
	if (connector.isConnected&&roomId==-1)
	{
		::pt::ReGetRoomList nwe;
		connector.sendNetworkEvent(::pt::reGetRoomList, nwe);
		::pt::DaRoomList drl;
		if (connector.getNetworkEvent(drl))
		{
			setInfo(drl.room);
		}
	}
}

void RoomGUI::show()
{
	sBackMenu.setPosition(200, 60);
	app->draw(sBackMenu);
	for (int i = 0; i < room.size(); i++)
	{
		sBackSelet.setPosition(210 + (i % 2) * 306, 80 + (i / 2) * 100);
		app->draw(sBackSelet);
		text.setString(L"��������: " + ::std::to_string(room[i].getNum()) + L"��");
		text.setPosition(215 + (i % 2) * 306, 85 + (i / 2) * 100);
		app->draw(text);
		button[i].app = app;
		button[i].setPosition(300 + i * 306, 85 + i * 100);
	}
}

int RoomGUI::onClick(::sf::Event& e)
{
	for (int i = 0; i < room.size(); i++)
	{
		if (button[i].onClick(e)&&room[i].getNum()<3)
		{
			::pt::ReJoinRoom nwe;
			if (connector.sendNetworkEvent(::pt::reJoinRoom, nwe))
				return room[i].getID();
			break;
		}
	}
	return -1;
}