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
	::std::vector<int> t(54,1);
	int n = 0, k;
	srand(time(0));
	while (n<54)
	{
		k = rand() % 54;
		if (t[k] == 1)
		{
			tempCard[n++] = k;
			t[k] = 0;
		}
	}
	for (int i = 0; i < players.size(); i++)
		players[i]->player.clear();
	isDealing = true;
	isDealDizhu = false;
	players[0]->player.isCallingDizhu = true;
}

void Game::update()
{
	//更新游戏状态（准备、发牌等阶段）
	::std::vector<Customor*>::iterator ct;
	for (ct = players.begin(); ct != players.end();)
	{
		if ((*ct) == nullptr || (*ct)->isConnected == false)
			ct = players.erase(ct);
		else
			ct++;
	}

	int n = 0;
	for (int i = 0; i < players.size(); i++)
		if (players[i]->player.isReady)
			n++;
	if (isPlaying && n < 3)
	{
		isPlaying = false;
		::pt::DaGameState dgs;
		dgs.gsta = Ready;
		::sf::Packet packet;
		packet << static_cast<int>(dgs.type()) << dgs;
		for (int i = 0; i < players.size(); i++)
			players[i]->sendNetworkEvent(packet);
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
		for (int i = 0; i < players.size(); i++)
			players[i]->sendNetworkEvent(packet);
		::std::cout << "send game state : deal\n";
	}

	//处理客户端请求
	::std::vector<Customor*>::iterator c;
	for (c = players.begin(); c != players.end();)
	{
		::pt::NetworkEvent* e = (*c)->proccessEvent();
		if (e == nullptr)
			c++;
			break;
		::pt::MSG_TYPE type = e->type();
		switch (type)
		{
		case pt::reNull:
			c++;
			break;
		case pt::reConnect:
			c++;
			break;
		case pt::reDisconnect:
			(*c)->disconnect();
			c = players.erase(c);
			break;
		case pt::reGetRoomList:
			c++;
			break;
		case pt::reCreatRoom:
			c++;
			break;
		case pt::reJoinRoom:
			c++;
			break;
		case pt::reReady:
		{
			(*c)->player.isReady = true;
			::pt::DaPlayerStateInfo_Ready dsr;
			for (int i = 0; i < players.size(); i++)
			{
				::std::pair<int, bool> a;
				a.first = players[i]->Id();
				a.second = players[i]->player.isReady;
				dsr.isReady.push_back(a);
			}
			::sf::Packet packet;
			packet << static_cast<int>(dsr.type()) << dsr;
			for (int i = 0; i < players.size(); i++)
				players[i]->sendNetworkEvent(packet);
			::std::cout << "get ready and send player state information\n";
			c++;
			break;
		}
		case pt::reUnReady:
		{
			(*c)->player.isReady = false;
			::pt::DaPlayerStateInfo_Ready dsr;
			for (int i = 0; i < players.size(); i++)
			{
				::std::pair<int, bool> a;
				a.first = players[i]->Id();
				a.second = players[i]->player.isReady;
				dsr.isReady.push_back(a);
			}
			::sf::Packet packet;
			packet << static_cast<int>(dsr.type()) << dsr;
			for (int i = 0; i < players.size(); i++)
				players[i]->sendNetworkEvent(packet);
			::std::cout << "get unready and send player state information\n";
			c++;
			break;
		}
		case pt::reExitRoom:
			c = players.erase(c);
			break;
			//清空出牌区
		case pt::reClearDeskCard:
		{
			::pt::DaDeskCard ddc;
			ddc.cards.resize(0);
			::sf::Packet packet;
			packet << static_cast<int>(ddc.type()) << ddc;
			(*c)->sendNetworkEvent(packet);
			c++;
			break;
		}
		//叫分
		case pt::daCallDec:
		{
			(*c)->player.isCallingDizhu = false;
			(*c)->player.s_call = static_cast<::pt::DaCallDec*>(e)->s_call;
			::pt::DaPlayerStateInfo_Call dpsc;
			dpsc.player_turned_id = (*c)->Id();
			dpsc.s_call = (*c)->player.s_call;
			::sf::Packet packet;
			packet << static_cast<int>(dpsc.type()) << dpsc;
			for (int i = 0; i < players.size(); i++)
				players[i]->sendNetworkEvent(packet);
			if (c + 1 != players.end())
				dpsc.player_turned_id = (*(c + 1))->Id();
			else
				dpsc.player_turned_id = (*players.begin())->Id();
			dpsc.s_call = -1;
			packet.clear();
			packet << static_cast<int>(dpsc.type()) << dpsc;
			for (int i = 0; i < players.size(); i++)
				players[i]->sendNetworkEvent(packet);
			c++;
			break;
		}
		//出牌
		case pt::daChuDec:
		{
			(*c)->player.dec = static_cast<::pt::DaChuDec*>(e)->dec;
			if ((*c)->player.dec == CHU)
			{
				::std::vector<int> cards = static_cast<::pt::DaChuDec*>(e)->cards;
				::pt::DaDeskCard dc;
				dc.cards = cards;
				::sf::Packet packet;
				packet << static_cast<int>(dc.type()) << dc;
				for (int i = 0; i < players.size(); i++)
					players[i]->sendNetworkEvent(packet);
				for (int i = 0; i < cards.size(); i++)
					(*c)->player.removeCard(cards[i]);
			}
			::pt::DaPlayerStateInfo_Chu dsc;
			dsc.dec = (*c)->player.dec;
			dsc.player_turned_id = (*c)->Id();
			for (int i = 0; i < players.size(); i++)
			{
				::std::pair<int, SF>a;
				a.first = players[i]->Id();
				a.second = players[i]->player.sid;
				::std::pair<int, ::std::vector<int>> b;
				b.first = players[i]->player.hand_card.size();
				b.second = players[i]->player.hand_card;
				::std::pair<::std::pair<int, SF>, ::std::pair<int, ::std::vector<int>>> c;
				dsc.playerInfo.push_back(c);
			}
			::sf::Packet packet;
			packet << static_cast<int>(dsc.type()) << dsc;
			for (int i = 0; i < players.size(); i++)
				players[i]->sendNetworkEvent(packet);
			if (c + 1 != players.end())
				dsc.player_turned_id = (*(c + 1))->Id();
			else
				dsc.player_turned_id = (*players.begin())->Id();
			dsc.dec = NOT;
			packet.clear();
			packet << static_cast<int>(dsc.type()) << dsc;
			for (int i = 0; i < players.size(); i++)
				players[i]->sendNetworkEvent(packet);
			c++;
			break;
		}
		case pt::daGameState:
			c++;
			break;
		case pt::daPlayerStateInfo_Ready:
			c++;
			break;
		case pt::daPlayerStateInfo_Call:
			c++;
			break;
		case pt::daPlayerStateInfo_Chu:
			c++;
			break;
		case pt::daBeishu:
			c++;
			break;
		case pt::daDizhuCard:
			c++;
			break;
		case pt::daDeskCard:
			c++;
			break;
		case pt::daGameOver:
			c++;
			break;
		case pt::daRoomList:
			c++;
			break;
		case pt::daDealCard:
			c++;
			break;
		default:
			c++;
			break;
		}
	}

	//游戏逻辑
	if(isPlaying)
		game_logic();
}

void Game::game_logic()
{
	if (isDealing)
	{
		::pt::DaGameState dgs;
		dgs.gsta = Deal;
		::sf::Packet packet;
		packet << static_cast<int>(dgs.type()) << dgs;
		for (int i = 0; i < players.size(); i++)
			players[i]->sendNetworkEvent(packet);

		int t = 0;
		while (tempCard.size() > 3)
		{
			::pt::DaDealCard ddc;
			ddc.playerId = players[t]->Id();
			ddc.cards.push_back(tempCard[tempCard.size() - 1]);
			players[t]->player.addCard(tempCard[tempCard.size() - 1]);
			tempCard.pop_back();
			::sf::Packet packet;
			packet << static_cast<int>(ddc.type()) << ddc;
			for (int i = 0; i < players.size(); i++)
				players[i]->sendNetworkEvent(packet);
			t = (t + 1) % 3;
		}
		::pt::DaDizhuCard dc;
		::std::sort(tempCard.begin(), tempCard.end(), [](int a, int b)->bool {return a < b; });
		dc.cards = tempCard;
		tempCard.clear();
		packet.clear();
		packet << static_cast<int>(dc.type()) << dc;
		for (int i = 0; i < players.size(); i++)
			players[i]->sendNetworkEvent(packet);
		isDealing = false;
		isDealDizhu = true;
		players[0]->player.isCallingDizhu = true;
		::pt::DaGameState dg;
		dg.gsta = Call;
		packet.clear();
		packet << static_cast<int>(dg.type()) << dg;
		for (int i = 0; i < players.size(); i++)
			players[i]->sendNetworkEvent(packet);
	}
	else if (isDealDizhu)
	{
		if (players[0]->player.s_call != -1 && players[1]->player.s_call != -1 && players[2]->player.s_call != -1)
		{
			int t = 0;
			for (int i = 0; i < players.size(); i++)
			{
				if (players[i]->player.s_call > players[t]->player.s_call)
					t = i;
			}
			if (players[t]->player.s_call == 0)
			{
				start();
				return;
			}
			players[t]->player.sid = DIZHU;
			players[(t+1)%3]->player.sid = NONM;
			players[(t+2)%3]->player.sid = NONM;
			::pt::DaDealCard ddc;
			ddc.playerId = players[t]->Id();
			for (int i = 0; i < 3; i++)
			{
				ddc.cards.push_back(dizhuCard[i]);
				players[t]->player.addCard(dizhuCard[i]);
			}
			::sf::Packet packet;
			packet << static_cast<int>(ddc.type()) << ddc;
			for (int i = 0; i < players.size(); i++)
				players[i]->sendNetworkEvent(packet);
			isDealDizhu = false;
			::pt::DaGameState dgs;
			dgs.gsta = Play;
			packet.clear();
			packet << static_cast<int>(dgs.type()) << dgs;
			for (int i = 0; i < players.size(); i++)
				players[i]->sendNetworkEvent(packet);
			::pt::DaPlayerStateInfo_Chu dsc;
			dsc.dec = players[t]->player.dec;
			dsc.player_turned_id = players[t]->Id();
			for (int i = 0; i < players.size(); i++)
			{
				::std::pair<int, SF>a;
				a.first = players[i]->Id();
				a.second = players[i]->player.sid;
				::std::pair<int, ::std::vector<int>> b;
				b.first = players[i]->player.hand_card.size();
				b.second = players[i]->player.hand_card;
				::std::pair<::std::pair<int, SF>, ::std::pair<int, ::std::vector<int>>> c;
				dsc.playerInfo.push_back(c);
			}
			packet.clear();
			packet << static_cast<int>(dsc.type()) << dsc;
			for (int i = 0; i < players.size(); i++)
				players[i]->sendNetworkEvent(packet);
		}
		else
		{
			::pt::DaPlayerStateInfo_Call dpsc;
			for (int i = 0; i < players.size(); i++)
			{
				if (players[i]->player.isCallingDizhu)
				{
					dpsc.player_turned_id = players[i]->Id();
					dpsc.s_call = players[i]->player.s_call;
					if (players[i]->player.s_call != -1)
					{
						players[i]->player.isCallingDizhu = false;
						players[(i + 1) % 3]->player.isCallingDizhu = true;
					}
					::sf::Packet packet;
					packet << static_cast<int>(dpsc.type()) << dpsc;
					for (int i = 0; i < players.size(); i++)
						players[i]->sendNetworkEvent(packet);
					break;
				}
			}
		}
	}
	else
	{
		//判断游戏是否结束
		int t = -1;
		for (int i = 0; i < players.size(); i++)
		{
			if (players[i]->player.hand_card.size()==0)
				t = i;
		}
		if (t != -1)
		{
			isPlaying = false;
			::pt::DaGameOver dgo;
			dgo.winner_id = players[t]->Id();
			::sf::Packet packet;
			packet << static_cast<int>(dgo.type()) << dgo;
			for (int i = 0; i < players.size(); i++)
				players[i]->sendNetworkEvent(packet);
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

void Game::addPlayer(Customor* c)
{
	players.push_back(c);
	::pt::DaGameState dgs;
	if (players.size() == 3)
		dgs.gsta = Deal;
	else
		dgs.gsta = Ready;
	::sf::Packet packet;
	packet << static_cast<int>(dgs.type()) << dgs;
	for (int i = 0; i < players.size(); i++)
		players[i]->sendNetworkEvent(packet);
	::pt::DaPlayerStateInfo_Ready dpsr;
	for (int i = 0; i < players.size(); i++)
		dpsr.isReady.push_back(::std::pair<int, bool>(players[i]->Id(), players[i]->player.isReady));
	packet.clear();;
	packet << static_cast<int>(dpsr.type()) << dpsr;
	for (int i = 0; i < players.size(); i++)
		players[i]->sendNetworkEvent(packet);
}

void Game::removePlayer(Customor* c)
{
	::std::vector<Customor*>::iterator itr;
	for (itr = players.begin(); itr != players.end();)
	{
		if ((*itr)->Id() == c->Id())
		{
			players.erase(itr);
			return;
		}
		itr++;
	}
}

int Game::getNum()
{
	return this->players.size();
}

bool Game::getState()
{
	return this->isPlaying;
}

int Game::getID()
{
	return this->ID;
}

::std::vector<Customor*> Game::getPlayersList()
{
	return this->players;
}