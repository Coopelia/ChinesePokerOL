#include"Server.h"
#include"PacketType.h"
#include"Game.h"
#include"Customor.h"

Server::Server()
{
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
}

void Server::update_games()
{
	//����ÿ�����䣬��������
	::std::list<Game* >::iterator itr;
	for (itr = games.begin(); itr != games.end(); itr++)
	{
		::std::vector<Customor*>::iterator it;
		for ( it = (*itr)->players.begin(); it !=(*itr)->players.end() ;)
		{
			if ((*it) == nullptr)
				it = (*itr)->players.erase(it);
			else
				it++;
		}
	}
}

void Server::Update()
{
	::std::list<Customor*>::iterator itr;
	mt_c.lock();
	for ( itr = clients.begin(); itr !=clients.end(); itr++)
	{
		//�����û�з��ʹ��������룩���������
		::pt::ReCreatRoom rcr;
		::pt::ReGetRoomList rgr;
		if ((*itr)->getNetworkEvent(rcr))//��������
		{
			Game* game = new Game();
			game->setID(games.size() + 1);
			game->addPlayer((*itr));
			::pt::DaPlayerStateInfo_Ready dpsr;
			dpsr.isReady.push_back(::std::pair<int, bool>((*itr)->Id(), true));
			(*itr)->sendNetworkEvent(::pt::daPlayerStateInfo_Ready, dpsr);
		}
		else if ((*itr)->getNetworkEvent(rgr))//��ȡ�����б�
		{
			::pt::DaRoomList drl;
			::std::list<Game*>::iterator it;
			for (it=games.begin();it!=games.end();it++)
				drl.game.push_back(*(*it));
			(*itr)->sendNetworkEvent(::pt::daRoomList, drl);
		}
		//���뷿��
		::pt::ReJoinRoom rjr(0);
		if ((*itr)->getNetworkEvent(rjr))
		{
			::std::list<Game*>::iterator it;
			for (it = games.begin(); it != games.end(); it++)
			{
				if ((*it)->getID() == rjr.roomId)
				{
					(*it)->addPlayer((*itr));
					::pt::DaPlayerStateInfo_Ready dpsr;
					::std::vector<Customor*> players = (*it)->getPlayersList();
					for (int i = 0; i < players.size(); i++)
						dpsr.isReady.push_back(::std::pair<int, bool>(players[i]->Id(), players[i]->player.isReady));
					(*itr)->sendNetworkEvent(::pt::daPlayerStateInfo_Ready, dpsr);
					break;
				}
			}
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