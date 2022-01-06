#include"Game.h"
#include"Customor.h"

Game::Game()
{
	ID = -1;
	isPlaying = false;
}

void Game::start()
{
	dizhuCard.resize(3, -1);
	tempCard.resize(54);
	::std::vector<int> t(54, 1);
	int n = 0, k;
	srand(time(0));
	while (n < 54)
	{
		k = rand() % 54;
		if (t[k] == 1)
		{
			tempCard[n++] = k;
			t[k] = 0;
		}
	}
	for (int i = 0; i < player_list.size(); i++)
		clients[player_list[i]]->player.clear();
	isDealing = true;
	isDealDizhu = false;
	clients[player_list[0]]->player.isCallingDizhu = true;
}

void Game::update()
{
	//更新游戏状态（准备、发牌等阶段）
	for (int i = 0; i < player_list.size(); i++)
	{
		if (clients[player_list[i]] == nullptr || clients[player_list[i]]->isConnected == false)
			removePlayer(player_list[i]);
	}

	int n = 0;
	for (int i = 0; i < player_list.size(); i++)
		if (clients[player_list[i]]->player.isReady)
			n++;
	if (isPlaying && n < 3)
	{
		isPlaying = false;
		::pt::DaGameState dgs;
		dgs.gsta = Ready;
		::sf::Packet packet;
		packet << static_cast<int>(dgs.type()) << dgs;
		for (int i = 0; i < player_list.size(); i++)
			clients[player_list[i]]->sendNetworkEvent(packet);
		::std::cout << "send game state : ready\n";
	}
	else if (!isPlaying && n == 3)
	{
		isPlaying = true;
		start();
		::pt::DaGameState dgs;
		dgs.gsta = Deal;
		::sf::Packet packet;
		packet << static_cast<int>(dgs.type()) << dgs;
		for (int i = 0; i < player_list.size(); i++)
			clients[player_list[i]]->sendNetworkEvent(packet);
		::std::cout << "send game state : deal\n";
	}

	//处理客户端请求
	for (int i = 0; i < player_list.size(); i++)
	{
		::pt::NetworkEvent* e = clients[player_list[i]]->proccessEvent();
		if (e == nullptr)
			continue;
		::pt::MSG_TYPE type = e->type();
		switch (type)
		{
		case pt::reNull:
			break;
		case pt::reConnect:
			break;
		case pt::reDisconnect:
			clients[player_list[i]]->disconnect();
			removePlayer(player_list[i]);
			break;
		case pt::reGetRoomList:
			break;
		case pt::reCreatRoom:
			break;
		case pt::reJoinRoom:
			break;
		case pt::reReady:
		{
			clients[player_list[i]]->player.isReady = true;
			::pt::DaPlayerStateInfo_Ready dsr;
			for (int j = 0; j < player_list.size(); j++)
			{
				::std::pair<int, bool> a;
				a.first = clients[player_list[j]]->Id();
				a.second = clients[player_list[j]]->player.isReady;
				dsr.isReady.push_back(a);
			}
			::sf::Packet packet;
			packet << static_cast<int>(dsr.type()) << dsr;
			for (int j = 0; j < player_list.size(); j++)
				clients[player_list[j]]->sendNetworkEvent(packet);
			::std::cout << "get ready and send player state information\n";
			break;
		}
		case pt::reUnReady:
		{
			clients[player_list[i]]->player.isReady = false;
			::pt::DaPlayerStateInfo_Ready dsr;
			for (int j = 0; j < player_list.size(); j++)
			{
				::std::pair<int, bool> a;
				a.first = clients[player_list[j]]->Id();
				a.second = clients[player_list[j]]->player.isReady;
				dsr.isReady.push_back(a);
			}
			::sf::Packet packet;
			packet << static_cast<int>(dsr.type()) << dsr;
			for (int j = 0; j < player_list.size(); j++)
				clients[player_list[j]]->sendNetworkEvent(packet);
			::std::cout << "get unready and send player state information\n";
			break;
		}
		case pt::reExitRoom:
			removePlayer(player_list[i]);
			break;
			//清空出牌区
		case pt::reClearDeskCard:
		{
			::pt::DaDeskCard ddc;
			ddc.cards.resize(0);
			::sf::Packet packet;
			packet << static_cast<int>(ddc.type()) << ddc;
			for (int j = 0; j < player_list.size(); j++)
				clients[player_list[j]]->sendNetworkEvent(packet);
			break;
		}
		//叫分
		case pt::daCallDec:
		{
			::std::cout << "recieve: call_dec\n";
			clients[player_list[i]]->player.isCallingDizhu = false;
			clients[player_list[i]]->player.s_call = static_cast<::pt::DaCallDec*>(e)->s_call;
			::pt::DaPlayerStateInfo_Call dpsc;
			dpsc.player_turned_id = clients[player_list[i]]->Id();
			dpsc.s_call = clients[player_list[i]]->player.s_call;
			::sf::Packet packet;
			packet << static_cast<int>(dpsc.type()) << dpsc;
			for (int j = 0; j < player_list.size(); j++)
				clients[player_list[j]]->sendNetworkEvent(packet);
			if (i < player_list.size() - 1)
			{
				dpsc.player_turned_id = clients[player_list[i + 1]]->Id();
				dpsc.s_call = clients[player_list[i + 1]]->player.s_call;
				clients[player_list[i + 1]]->player.isCallingDizhu = true;
			}
			else
			{
				dpsc.player_turned_id = clients[player_list[0]]->Id();
				clients[player_list[0]]->player.isCallingDizhu = true;
				dpsc.s_call = clients[player_list[0]]->player.s_call;
			}
			packet.clear();
			packet << static_cast<int>(dpsc.type()) << dpsc;
			for (int j = 0; j < player_list.size(); j++)
				clients[player_list[j]]->sendNetworkEvent(packet);
			break;
		}
		//出牌
		case pt::daChuDec:
		{
			clients[player_list[i]]->player.dec = static_cast<::pt::DaChuDec*>(e)->dec;
			if (clients[player_list[i]]->player.dec == CHU)
			{
				::std::vector<int> cards = static_cast<::pt::DaChuDec*>(e)->cards;
				::pt::DaDeskCard dc;
				dc.cards = cards;
				::sf::Packet packet;
				packet << static_cast<int>(dc.type()) << dc;
				for (int j = 0; j < player_list.size(); j++)
					clients[player_list[j]]->sendNetworkEvent(packet);
				for (int j = 0; j < cards.size(); j++)
					clients[player_list[i]]->player.removeCard(cards[j]);
			}
			send_player_info();
			clients[player_list[i]]->player.isMyTime = false;
			if (i + 1 != player_list.size())
			{
				clients[player_list[i + 1]]->player.dec = NOT;
				clients[player_list[i + 1]]->player.isMyTime = true;
			}
			else
			{
				clients[player_list[0]]->player.dec = NOT;
				clients[player_list[0]]->player.isMyTime = true;
			}
			send_player_info();
			break;
		}
		case pt::daGameState:
			break;
		case pt::daPlayerStateInfo_Ready:
			break;
		case pt::daPlayerStateInfo_Call:
			break;
		case pt::daPlayerStateInfo_Chu:
			break;
		case pt::daBeishu:
			break;
		case pt::daDizhuCard:
			break;
		case pt::daDeskCard:
			break;
		case pt::daGameOver:
			break;
		case pt::daRoomList:
			break;
		case pt::daDealCard:
			break;
		default:
			break;
		}
	}

	//游戏逻辑
	if (isPlaying)
		game_logic();
}

void Game::send_player_info()
{
	int t = -1;
	for (int i = 0; i < player_list.size(); i++)
	{
		if (clients[player_list[i]]->player.isMyTime)
			t = i;
	}
	::pt::DaPlayerStateInfo_Chu dsc;
	if (t != -1)
	{
		dsc.player_turned_id = clients[player_list[t]]->Id();
		dsc.dec = clients[player_list[t]]->player.dec;
	}
	else
	{
		dsc.player_turned_id = -1;
		dsc.dec = NOT;
	}
	for (int i = 0; i < player_list.size(); i++)
	{
		::std::pair<::std::pair<int, SF>, ::std::pair<int, ::std::vector<int>>> c;
		c.first.first = clients[player_list[i]]->Id();
		c.first.second = clients[player_list[i]]->player.sid;
		c.second.first = clients[player_list[i]]->player.hand_card.size();
		c.second.second = clients[player_list[i]]->player.hand_card;
		dsc.playerInfo.push_back(c);
	}
	::sf::Packet packet;
	packet << static_cast<int>(dsc.type()) << dsc;
	for (int i = 0; i < player_list.size(); i++)
		clients[player_list[i]]->sendNetworkEvent(packet);
}

void Game::game_logic()
{
	if (isDealing)
	{
		::pt::DaGameState dgs;
		dgs.gsta = Deal;
		::sf::Packet packet;
		packet << static_cast<int>(dgs.type()) << dgs;
		for (int i = 0; i < player_list.size(); i++)
			clients[player_list[i]]->sendNetworkEvent(packet);

		int t = 0;
		while (tempCard.size() > 3)
		{
			/*::pt::DaDealCard ddc;
			ddc.playerId = clients[player_list[t]]->Id();
			ddc.cards.push_back(tempCard[tempCard.size() - 1]);*/
			clients[player_list[t]]->player.addCard(tempCard[tempCard.size() - 1]);
			tempCard.pop_back();
			/*::sf::Packet packet;
			packet << static_cast<int>(ddc.type()) << ddc;
			for (int i = 0; i < player_list.size(); i++)
				clients[player_list[i]]->sendNetworkEvent(packet);*/
			t = (t + 1) % 3;
			send_player_info();
		}

		::pt::DaDizhuCard dc;
		::std::sort(tempCard.begin(), tempCard.end(), [](int a, int b)->bool {return a < b; });
		dc.cards = tempCard;
		dizhuCard = tempCard;
		tempCard.clear();
		packet.clear();
		packet << static_cast<int>(dc.type()) << dc;
		for (int i = 0; i < player_list.size(); i++)
			clients[player_list[i]]->sendNetworkEvent(packet);
		isDealing = false;
		isDealDizhu = true;
		clients[player_list[0]]->player.isCallingDizhu = true;
		clients[player_list[0]]->player.s_call = -1;

		::pt::DaPlayerStateInfo_Call dpsc;
		dpsc.player_turned_id = clients[player_list[0]]->Id();
		dpsc.s_call = clients[player_list[0]]->player.s_call;
		packet.clear();
		packet << static_cast<int>(dpsc.type()) << dpsc;
		for (int j = 0; j < player_list.size(); j++)
			clients[player_list[j]]->sendNetworkEvent(packet);

		dgs.gsta = Call;
		packet.clear();
		packet << static_cast<int>(dgs.type()) << dgs;
		for (int i = 0; i < player_list.size(); i++)
			clients[player_list[i]]->sendNetworkEvent(packet);
	}
	else if (isDealDizhu)
	{
		if (clients[player_list[0]]->player.s_call != -1 && clients[player_list[1]]->player.s_call != -1 && clients[player_list[2]]->player.s_call != -1)
		{
			int t = 0;
			for (int i = 0; i < player_list.size(); i++)
			{
				clients[player_list[i]]->player.isCallingDizhu = false;
				if (clients[player_list[i]]->player.s_call > clients[player_list[t]]->player.s_call)
					t = i;
			}

			clients[player_list[t]]->player.sid = DIZHU;
			clients[player_list[t]]->player.isMyTime = true;
			clients[player_list[(t + 1) % 3]]->player.sid = NONM;
			clients[player_list[(t + 2) % 3]]->player.sid = NONM;
			/*::pt::DaDealCard ddc;
			ddc.playerId = clients[player_list[t]]->Id();*/
			for (int i = 0; i < 3; i++)
			{
				//ddc.cards.push_back(dizhuCard[i]);
				clients[player_list[t]]->player.addCard(dizhuCard[i]);
			}
			::sf::Packet packet;
			/*packet << static_cast<int>(ddc.type()) << ddc;
			for (int i = 0; i < player_list.size(); i++)
				clients[player_list[i]]->sendNetworkEvent(packet);*/
			isDealDizhu = false;
			::pt::DaGameState dgs;
			dgs.gsta = Play;
			packet.clear();
			packet << static_cast<int>(dgs.type()) << dgs;
			for (int i = 0; i < player_list.size(); i++)
				clients[player_list[i]]->sendNetworkEvent(packet);

			send_player_info();
		}
	}
	else
	{
		//判断游戏是否结束
		int t = -1;
		for (int i = 0; i < player_list.size(); i++)
		{
			if (clients[player_list[i]]->player.hand_card.size() == 0)
				t = i;
		}
		if (t != -1)
		{
			isPlaying = false;
			::pt::DaGameOver dgo;
			dgo.winner_id = clients[player_list[t]]->Id();
			::sf::Packet packet;
			packet << static_cast<int>(dgo.type()) << dgo;
			for (int i = 0; i < player_list.size(); i++)
			{
				clients[player_list[i]]->player.isReady = false;
				clients[player_list[i]]->sendNetworkEvent(packet);
			}
		}
	}
}

void Game::setState(bool isPlaying)
{
	this->isPlaying = isPlaying;
}

void Game::setID(int id)
{
	this->ID = id;
}

void Game::addPlayer(int id)
{
	player_list.push_back(id);
	::pt::DaGameState dgs;
	if (player_list.size() == 3)
		dgs.gsta = Deal;
	else
		dgs.gsta = Ready;
	::sf::Packet packet;
	packet << static_cast<int>(dgs.type()) << dgs;
	for (int i = 0; i < player_list.size(); i++)
		clients[player_list[i]]->sendNetworkEvent(packet);
	::pt::DaPlayerStateInfo_Ready dpsr;
	for (int i = 0; i < player_list.size(); i++)
		dpsr.isReady.push_back(::std::pair<int, bool>(clients[player_list[i]]->Id(), clients[player_list[i]]->player.isReady));
	packet.clear();
	packet << static_cast<int>(dpsr.type()) << dpsr;
	for (int i = 0; i < player_list.size(); i++)
		clients[player_list[i]]->sendNetworkEvent(packet);
}

void Game::removePlayer(int id)
{
	::std::vector<int>::iterator itr;
	for (itr = player_list.begin(); itr != player_list.end();)
	{
		if (id == *itr)
		{
			player_list.erase(itr);
			return;
		}
		itr++;
	}
}

int Game::getNum()
{
	return this->player_list.size();
}

bool Game::getState()
{
	return this->isPlaying;
}

int Game::getID()
{
	return this->ID;
}

::std::vector<int> Game::getPlayersList()
{
	return this->player_list;
}