#include"Server.h"
#include"PacketType.h"
#include"Game.h"
#include"Customor.h"

Server::Server()
{
	::std::cout << "server is running, waiting for the customor.\n";
}

Server::~Server()
{
	for (auto p : games)
	{
		delete p;
	}
	for(auto p : clients)
	{
		delete p;
	}
}

void Server::Start()
{
	connector.runThreads(&connector);
	::std::list<Game*>::iterator itr;
	for (itr = games.begin(); itr != games.end(); itr++)
		(*itr)->start();
}

void Server::update_games()
{
	//遍历每个房间，更新数据
	::std::list<Game* >::iterator itr;
	for (itr = games.begin(); itr != games.end();)
	{
		if ((*itr)->player_list.size() == 0)
			itr = games.erase(itr);
		else
		{
			(*itr)->update();
			itr++;
		}
	}
}

void Server::Update()
{
	mt_c.lock();
	for ( int i=0;i<clients.size();i++)
	{
		//玩家有没有发送创建（加入）房间的请求
		::pt::NetworkEvent* rcr = nullptr;
		if (clients[i]->getNetworkEvent(::pt::reCreatRoom,rcr))//创建房间
		{
			::std::cout << "create a game\n";
			Game* game = new Game();
			game->setID(games.size() + 1);
			game->addPlayer(i);
			games.push_back(game);
			::pt::DaPlayerStateInfo_Ready dpsr;
			dpsr.isReady.push_back(::std::pair<int, bool>(clients[i]->Id(), true));
			::sf::Packet packet;
			packet << static_cast<int>(dpsr.type()) << dpsr;
			clients[i]->sendNetworkEvent(packet);
			delete rcr;
		}
		if (clients[i]->getNetworkEvent(::pt::reGetRoomList,rcr))//获取房间列表
		{
			::std::cout << "send game list\n";
			::pt::DaRoomList drl;
			::sf::Packet packet;
			::std::list<Game*>::iterator it;
			for (it = games.begin(); it != games.end(); it++)
				drl.game.push_back((*it));
			packet << static_cast<int>(drl.type()) << drl;
			clients[i]->sendNetworkEvent(packet);
			delete rcr;
		}
		//加入房间
		::pt::NetworkEvent* rjr = nullptr;
		if (clients[i]->getNetworkEvent(::pt::reJoinRoom, rjr))
		{
			::std::list<Game*>::iterator it;
			for (it = games.begin(); it != games.end(); it++)
			{
				if ((*it)->getID() == static_cast<::pt::ReJoinRoom*>(rjr)->roomId)
				{
					::pt::ReJoinRoom re(0);
					re.roomId = (*it)->getID();
					::sf::Packet pt;
					pt << static_cast<int>(re.type()) << re;
					clients[i]->sendNetworkEvent(pt);
					(*it)->addPlayer(i);
					break;
				}
			}
			delete rjr;
		}
	}
	mt_c.unlock();
	update_games();
}

void Server::Run()
{
	Start();
	while (true)
	{
		Update();
	}
}