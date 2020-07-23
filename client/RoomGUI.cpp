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
	this->bt_update.setTextrue("assets/image/game/��������/#��ť/ˢ��.png");
	this->bt_update.setScale(0.5, 0.5);
	this->bt_update.setPosition(270, 90);
	//// debug///////
	/*this->room.resize(4);
	this->button.resize(4);
	for (int i = 0; i < room.size(); i++)
	{
		button[i].setTextrue("assets/image/game/��������/#��ť/bt_join.png");
		button[i].setScale(0.5, 0.5);
	}*/
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
	{
		button[i].setTextrue("assets/image/game/��������/#��ť/bt_join.png");
		button[i].setScale(0.5, 0.5);
	}
}

void RoomGUI::update_room_list()
{
	::pt::ReGetRoomList nwe;
	::sf::Packet packet;
	packet << static_cast<int>(nwe.type()) << nwe;
	connector.sendNetworkEvent(packet);
}

void RoomGUI::show()
{
	::pt::NetworkEvent* drl = nullptr;
	if (connector.getNetworkEvent(::pt::daRoomList, drl, true))
	{
		setInfo(static_cast<::pt::DaRoomList*>(drl)->room);
		::std::cout << "��ȡ���˷����б�\n";
		::std::cout << "��ǰ����" << room.size() << "������\n";
		delete drl;
	}

	sBackMenu.setPosition(200, 60);
	app->draw(sBackMenu);
	for (int i = 0; i < room.size(); i++)
	{
		sBackSelet.setPosition(300 + (i % 2) * 350, 180 + (i / 2) * 110);
		app->draw(sBackSelet);
		text.setString(L"��������: " + ::std::to_string(room[i].getNum()) + L"��");
		text.setPosition(320 + (i % 2) * 350, 190 + (i / 2) * 110);
		app->draw(text);
		button[i].app = app;
		button[i].setPosition(470 + (i % 2) * 350, 215 + (i / 2) * 110);
		button[i].show();
	}
	bt_update.app = app;
	bt_update.show();
}

int RoomGUI::onClick(::sf::Event& e)
{
	for (int i = 0; i < room.size(); i++)
	{
		if (button[i].onClick(e)&&room[i].getNum()<3)
		{
			::pt::ReJoinRoom nwe(room[i].getID());
			::sf::Packet packet;
			packet << static_cast<int>(nwe.type()) << nwe;
			if (connector.sendNetworkEvent(packet))
			{
				isLoading = true;
				::std::cout << "���ڼ���\n";
				::pt::NetworkEvent* re = nullptr;
				while (!connector.getNetworkEvent(::pt::reJoinRoom, re, true));
				isLoading = false;
				int id = static_cast<::pt::ReJoinRoom*>(re)->roomId;
				return id;
			}
			break;
		}
	}
	return -1;
}