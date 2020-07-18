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
}

void Game::update()
{
	//更新游戏状态（准备、发牌等阶段）
	int n = 0;
	for (int i = 0; i < players.size(); i++)
		if (players[i]->player.isReady)
			n++;
	if (isPlaying && n < 3)
	{
		isPlaying = false;
		::pt::DaGameState dgs;
		dgs.gsta = Ready;
		for (int i = 0; i < players.size(); i++)
			players[i]->sendNetworkEvent(::pt::daGameState, dgs);
	}
	else if (!isPlaying && n == 3)
	{
		isPlaying = true;
		start();
	}

	//处理客户端请求
	::std::vector<Customor*>::iterator c;
	for (c = players.begin(); c != players.end();)
	{
		::pt::NetworkEvent* e = (*c)->proccessEvent();
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
			for (int i = 0; i < players.size(); i++)
				players[i]->sendNetworkEvent(::pt::daPlayerStateInfo_Ready, dsr);
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
			for (int i = 0; i < players.size(); i++)
				players[i]->sendNetworkEvent(::pt::daPlayerStateInfo_Ready, dsr);
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
			(*c)->sendNetworkEvent(::pt::daDealCard, ddc);
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
			for (int i = 0; i < players.size(); i++)
				players[i]->sendNetworkEvent(::pt::daPlayerStateInfo_Call, dpsc);
			if (c + 1 != players.end())
				dpsc.player_turned_id = (*(c + 1))->Id();
			else
				dpsc.player_turned_id = (*players.begin())->Id();
			dpsc.s_call = -1;
			for (int i = 0; i < players.size(); i++)
				players[i]->sendNetworkEvent(::pt::daPlayerStateInfo_Call, dpsc);
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
				for (int i = 0; i < players.size(); i++)
					players[i]->sendNetworkEvent(::pt::daDeskCard, dc);
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
			for (int i = 0; i < players.size(); i++)
				players[i]->sendNetworkEvent(::pt::daPlayerStateInfo_Chu, dsc);
			if (c + 1 != players.end())
				dsc.player_turned_id = (*(c + 1))->Id();
			else
				dsc.player_turned_id = (*players.begin())->Id();
			dsc.dec = NOT;
			for (int i = 0; i < players.size(); i++)
				players[i]->sendNetworkEvent(::pt::daPlayerStateInfo_Chu, dsc);
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
		for (int i = 0; i < players.size(); i++)
			players[i]->sendNetworkEvent(::pt::daGameState, dgs);
		int t = 0;
		while (tempCard.size() > 3)
		{
			::pt::DaDealCard ddc;
			ddc.playerId = players[t]->Id();
			ddc.cards.push_back(tempCard[tempCard.size() - 1]);
			tempCard.pop_back();
			for (int i = 0; i < players.size(); i++)
				players[i]->sendNetworkEvent(::pt::daDealCard, ddc);
			t = (t + 1) % 3;
		}
		::pt::DaDizhuCard dc;
		::std::sort(tempCard.begin(), tempCard.end(), [](int a, int b)->bool {return a < b; });
		dc.cards = tempCard;
		tempCard.clear();
		for (int i = 0; i < players.size(); i++)
			players[i]->sendNetworkEvent(::pt::daDizhuCard, dc);
		isDealing = false;
		isDealDizhu = true;
		::pt::DaGameState dgs;
		dgs.gsta = Call;
		for (int i = 0; i < players.size(); i++)
			players[i]->sendNetworkEvent(::pt::daGameState, dgs);
	}
	else if (isDealDizhu)
	{
		
		isDealDizhu
	}
	else
	{
		::pt::DaGameState dgs;
		dgs.gsta = Play;
		for (int i = 0; i < players.size(); i++)
			players[i]->sendNetworkEvent(::pt::daGameState, dgs);
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

void Game::addPlayer(Customor*& c)
{
	players.push_back(c);

}

void Game::removePlayer(Customor*& c)
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