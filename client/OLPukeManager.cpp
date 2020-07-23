#include"OLPukeManager.h"

OLPukeManager::OLPukeManager()
{
	puke_dt_x = 0;
	puke_dt_e = 0;
	puke_chupai_lx = 640;
	puke_chupai_dx = 50;
	num_desk = 0;
	num_seleted = 0;
	this->human_self = NULL;
	this->human_1 = NULL;
	this->human_2 = NULL;
	for (int i = 0; i < 21; i++)
	{
		deskCard[i] = -1;
		seletedCard[i] = -1;
	}
}

void OLPukeManager::start()
{
	this->clearAll();
}

void OLPukeManager::update()
{
	//更新房间中玩家的状态信息
	::pt::NetworkEvent* dpsic = nullptr;
	if (connector.getNetworkEvent(::pt::daPlayerStateInfo_Chu, dpsic, false))
	{
		if (static_cast<::pt::DaPlayerStateInfo_Chu*>(dpsic)->player_turned_id != -1)
		{
			int pid = static_cast<::pt::DaPlayerStateInfo_Chu*>(dpsic)->player_turned_id;

			if (pid == human_self->playerId)
			{
				human_self->isMyTime = true;
				human_1->isMyTime = false;
				human_2->isMyTime = false;
				human_self->dec = static_cast<::pt::DaPlayerStateInfo_Chu*>(dpsic)->dec;
			}
			else if (pid == human_1->playerId)
			{
				human_self->isMyTime = false;
				human_1->isMyTime = true;
				human_2->isMyTime = false;
				human_1->dec = static_cast<::pt::DaPlayerStateInfo_Chu*>(dpsic)->dec;
			}
			else if (pid == human_2->playerId)
			{
				human_self->isMyTime = false;
				human_1->isMyTime = false;
				human_2->isMyTime = true;
				human_2->dec = static_cast<::pt::DaPlayerStateInfo_Chu*>(dpsic)->dec;
			}

			if (pid != player_turned_id)
			{
				::std::cout << "player turned :id--" << pid << "\n";
				if (pid == human_self->playerId)
				{
					human_self->clock_daojishi.restart();
				}
				else if (pid == human_1->playerId)
				{
					human_1->clock_daojishi.restart();
				}
				else if (pid == human_2->playerId)
				{
					human_2->clock_daojishi.restart();
				}
				player_turned_id = pid;
			}
		}

		//更新玩家手牌和身份
		::std::vector<::std::pair<::std::pair<int, SF>, ::std::pair<int, ::std::vector<int>>>> info = static_cast<::pt::DaPlayerStateInfo_Chu*>(dpsic)->playerInfo;
		for (auto p : info)
		{
			if (p.first.first == human_self->playerId)
			{
				//::std::cout << "human_self id" << p.first.first << " cards: ";
				human_self->num_card = p.second.first;
				human_self->sid = p.first.second;
				for (int i = 0; i < 20; i++)
					human_self->hand_card[i] = -1;
				for (int i = 0; i < human_self->num_card; i++)
				{
					human_self->hand_card[i] = p.second.second[i];
					//::std::cout << p.second.second[i] << " ";
				}
				//::std::cout << "\n";
			}
			else if (p.first.first == human_1->playerId)
			{
				//::std::cout << "human_right id" << p.first.first << " cards: ";
				human_1->num_card = p.second.first;
				human_1->sid = p.first.second;
				for (int i = 0; i < 20; i++)
					human_1->hand_card[i] = -1;
				for (int i = 0; i < human_1->num_card; i++)
				{
					human_1->hand_card[i] = p.second.second[i];
					//::std::cout << p.second.second[i] << " ";
				}
				//::std::cout << "\n";
			}
			else if (p.first.first == human_2->playerId)
			{
				//::std::cout << "human_left id" << p.first.first << " cards: ";
				human_2->num_card = p.second.first;
				human_2->sid = p.first.second;
				for (int i = 0; i < 20; i++)
					human_2->hand_card[i] = -1;
				for (int i = 0; i < human_2->num_card; i++)
				{
					human_2->hand_card[i] = p.second.second[i];
					//::std::cout << p.second.second[i] << " ";
				}
				//::std::cout << "\n";
			}
		}

		//自己连续二回合时，清空出牌区的牌
		if ((human_self->isMyTime && human_self->dec == NOT && human_1->dec == PASS && human_2->dec == PASS) ||
			(human_1->isMyTime && human_1->dec == NOT && human_self->dec == PASS && human_2->dec == PASS) ||
			(human_2->isMyTime && human_2->dec == NOT && human_1->dec == PASS && human_self->dec == PASS))
		{
			::pt::ReClearDeskCard rcdc;
			::sf::Packet packet;
			packet << static_cast<int>(rcdc.type()) << rcdc;
			if (connector.sendNetworkEvent(packet))
			{
				for (int k = 0; k < num_desk; k++)
					deskCard[k] = -1;
				num_desk = 0;
			}
		}
		delete dpsic;
	}
	::pt::NetworkEvent* dc = nullptr;
	if (connector.getNetworkEvent(::pt::daDeskCard, dc, false))
	{
		//更新出牌区
		::std::vector<int>cards = static_cast<::pt::DaDeskCard*>(dc)->cards;
		for (int i = 0; i < 20; i++)
			deskCard[i] = -1;
		num_desk = cards.size();
		for (int i = 0; i < num_desk; i++)
			deskCard[i] = cards[i];
		int i, j;
		for (int k = 0; k < num_desk; k++)
		{
			i = deskCard[k] / 4;
			j = deskCard[k] % 4;
			puke.c[i][j].isDeleted = true;
			puke.c[i][j].isOnDesk = true;
			puke.c[i][j].isPressed = false;
			puke.c[i][j].isSeleted = false;
		}
		//更新地主牌颜色
		for (i = 0; i < 14; i++)
		{
			for (j = 0; j < 4; j++)
			{
				if (puke.c[i][j].isDeleted || puke.c[i][j].isOnDesk)
				{
					for (int k = 0; k < num_desk; k++)
					{
						if (dizhuCard[k] == i * 4 + j)
							sDizhuCard[k].sprite.setColor(Color(125, 125, 125, 125));
					}
				}
			}
		}
		delete dc;
	}

	//更新出牌区偏移量
	puke_chupai_dx = 50 - num_desk;
	puke_chupai_lx = 620 - num_desk * puke_chupai_dx / 2;

	//更新self手牌位置坐标偏移量
	puke_dt_e = 55 - human_self->num_card;
	puke_dt_x = 620 - human_self->num_card * puke_dt_e / 2;

	//更新牌的状态
	for (int i = 0; i < 14; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			puke.c[i][j].dt_e = puke_dt_e;
			puke.c[i][j].isOnTop = false;
			if (!mouseRect.isMousePressed)
				if (puke.c[i][j].isPressed)
				{
					puke.c[i][j].isSeleted = !puke.c[i][j].isSeleted;
					puke.c[i][j].isPressed = false;
				}
		}
	}

	puke.c[human_self->hand_card[human_self->num_card - 1] / 4][human_self->hand_card[human_self->num_card - 1] % 4].isOnTop = true;

	//更新出牌区牌的坐标
	for (int i = 0; i < num_desk; i++)
	{
		int k = deskCard[i] / 4;
		int l = deskCard[i] % 4;
		puke.c[k][l].sprite.setPosition(puke_chupai_lx + i * puke_chupai_dx, 280);
		puke.c[k][l].sprite.setScale(0.5, 0.5);
	}

	//更新自己牌的坐标
	for (int i = 0; i < human_self->num_card; i++)
	{
		int k = human_self->hand_card[i] / 4;
		int l = human_self->hand_card[i] % 4;
		int posY;
		if (puke.c[k][l].isSeleted)
			posY = 470;
		else
			posY = 520;
		puke.c[k][l].sprite.setPosition(puke_dt_x + i * puke_dt_e, posY);
	}
}

void OLPukeManager::clearAll()
{
	puke_dt_x = 0;
	puke_dt_e = 0;
	puke_chupai_lx = 640;
	puke_chupai_dx = 50;
	num_desk = 0;
	num_seleted = 0;
	for (int i = 0; i < 20; i++)
	{
		human_self->hand_card[i] = -1;
		human_1->hand_card[i] = -1;
		human_2->hand_card[i] = -1;
	}
	for (int i = 0; i < 21; i++)
	{
		deskCard[i] = -1;
		seletedCard[i] = -1;
	}
	human_self->num_card = 0;
	human_1->num_card = 0;
	human_2->num_card = 0;

	player_turned_id = 0;
}

void OLPukeManager::deal()
{
	/*::pt::NetworkEvent* ddc = nullptr;
	if (connector.getNetworkEvent(::pt::daDealCard, ddc, false))
	{
		if (static_cast<::pt::DaDealCard*>(ddc)->playerId == human_self->playerId)
		{
			for (int i = 0; i < static_cast<::pt::DaDealCard*>(ddc)->cards.size(); i++)
				human_self->addCard(static_cast<::pt::DaDealCard*>(ddc)->cards[i]);
		}
		else if (static_cast<::pt::DaDealCard*>(ddc)->playerId == human_1->playerId)
		{
			for (int i = 0; i < static_cast<::pt::DaDealCard*>(ddc)->cards.size(); i++)
				human_1->addCard(static_cast<::pt::DaDealCard*>(ddc)->cards[i]);
		}
		else if (static_cast<::pt::DaDealCard*>(ddc)->playerId == human_2->playerId)
		{
			for (int i = 0; i < static_cast<::pt::DaDealCard*>(ddc)->cards.size(); i++)
				human_2->addCard(static_cast<::pt::DaDealCard*>(ddc)->cards[i]);
		}
		delete ddc;
	}*/

	::pt::NetworkEvent* dic = nullptr;
	if (connector.getNetworkEvent(::pt::daDizhuCard, dic, false))
	{
		for (int i = 0; i < 3; i++)
		{
			dizhuCard[i] = static_cast<::pt::DaDizhuCard*>(dic)->cards[i];
			sDizhuCard[i].sprite = puke.c[dizhuCard[i] / 4][dizhuCard[i] % 4].sprite;
		}
		delete dic;
	}
}

void OLPukeManager::addToJudge(int i)
{
	seletedCard[num_seleted++] = i;
}

PukeType OLPukeManager::checkType(int* card, int num)
{
	if (num == 0)
		return illegal;
	else if (num == 2 && card[0] == 52 && card[1] == 53)
		return rocket;
	else if (num == 4 && card[0] / 4 == card[3] / 4)
		return bomb;
	else
	{
		int** temp = new int* [4];
		for (int i = 0; i < 4; i++)
		{
			temp[i] = new int[2];
			temp[i][0] = 0;
			temp[i][1] = 0;
		}
		User::analyse_card(card, num, temp);
		int list[4][2] = { 0 };
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 2; j++)
				list[i][j] = temp[i][j];
		delete[] temp;
		if (list[0][0] == 4)//四带
		{
			if (list[0][1] == 1)
			{
				if (num == 6)//四带二
					return four_single;
				else if (num == 8 && list[1][0] == 2 && list[1][1] == 2)//四带二对
					return four_couple;
				else
					return illegal;
			}
			else
				return illegal;
		}
		else if (((card[0] / 4) + list[0][1] - 1) == ((card[list[0][0] * list[0][1] - 1]) / 4))
		{
			if (list[0][0] == 3)//3连
			{
				if (list[0][1] == 1)//一个3连
				{
					if (num == 3)//不带
						return three;
					else if (num == 4)//三带一
						return three_single;
					else if (num == 5 && list[1][0] == 2)//三带二
						return three_couple;
					else
						return illegal;
				}
				else if (list[0][1] == 2)//2个三连
				{
					if (num == 6 && card[5] < 49)//不带
						return three_list2;
					else if (num == 8 && card[5] < 49)//带2个单
						return three_list2_single;
					else if (num == 10 && list[1][0] == 2 && card[5] < 49)//带2对
						return three_list2_couple;
					else
						return illegal;
				}
				else if (list[0][1] == 3)//3个三连
				{
					if (num == 9 && card[8] < 48)//不带
						return three_list3;
					else if (num == 12 && card[8] < 48)//带3个单
						return three_list3_single;
					else if (num == 15 && list[1][0] == 2 && list[1][1] == 3 && card[8] < 48)//带3对
						return three_list2_couple;
					else
						return illegal;
				}
				else if (list[0][1] == 4)//4个三连
				{
					if (num == 12 && card[11] < 48)//不带
						return three_list3;
					else if (num == 16 && card[11] < 48)//带4个单
						return three_list3_single;
					else if (num == 20 && list[1][0] == 2 && list[1][1] == 4 && card[11] < 48)//带4对
						return three_list4_couple;
					else
						return illegal;
				}
				else if (list[0][1] == 5)//5个三连
				{
					if (num == 15 && card[14] < 48)//不带
						return three_list5;
					else if (num == 20 && card[14] < 48)//带5个单
						return three_list5_single;
					else
						return illegal;
				}
				else if (list[0][1] == 6)//6个三连
				{
					if (num == 18 && card[17] < 48)//不带
						return three_list6;
					else
						return illegal;
				}
			}
			else if (list[0][0] == 2 && num == list[0][1] * 2)//对子
			{
				if (list[0][1] == 1)
					return couple;
				else if (list[0][1] == 2)
					return illegal;
				else if (list[0][1] == 3)
				{
					if (list[1][0] == 0 && card[num - 1] < 48)
						return couple_list3;
					else
						return illegal;
				}
				else if (list[0][1] == 4 && card[num - 1] < 48)
				{
					if (list[1][0] == 0)
						return couple_list4;
					else
						return illegal;
				}
				else if (list[0][1] == 5 && card[num - 1] < 48)
				{
					if (list[1][0] == 0)
						return couple_list5;
					else
						return illegal;
				}
				else if (list[0][1] == 6 && card[num - 1] < 48)
				{
					if (list[1][0] == 0)
						return couple_list6;
					else
						return illegal;
				}
				else if (list[0][1] == 7 && card[num - 1] < 48)
				{
					if (list[1][0] == 0)
						return couple_list7;
					else
						return illegal;
				}
				else if (list[0][1] == 8 && card[num - 1] < 48)
				{
					if (list[1][0] == 0)
						return couple_list8;
					else
						return illegal;
				}
				else if (list[0][1] == 9 && card[num - 1] < 48)
				{
					if (list[1][0] == 0)
						return couple_list9;
					else
						return illegal;
				}
				else if (list[0][1] == 10 && card[num - 1] < 48)
				{
					if (list[1][0] == 0)
						return couple_list10;
					else
						return illegal;
				}
				else
					return illegal;
			}
			else if (list[0][0] == 1)//单
			{
				if (list[0][1] == 1)
					return single;
				else if (list[0][1] == 5 && card[num - 1] < 48)
					return single_list5;
				else if (list[0][1] == 6 && card[num - 1] < 48)
					return single_list6;
				else if (list[0][1] == 7 && card[num - 1] < 48)
					return single_list7;
				else if (list[0][1] == 8 && card[num - 1] < 48)
					return single_list8;
				else if (list[0][1] == 9 && card[num - 1] < 48)
					return single_list9;
				else if (list[0][1] == 10 && card[num - 1] < 48)
					return single_list10;
				else if (list[0][1] == 11 && card[num - 1] < 48)
					return single_list11;
				else if (list[0][1] == 12 && card[num - 1] < 48)
					return single_list12;
				else
					return illegal;
			}
			else
				return illegal;
		}
		else
			return illegal;
	}
	return illegal;
}

void OLPukeManager::sort_seleted()
{
	int m, j, flag;
	int temp;
	m = num_seleted - 1;
	flag = 1;
	while ((m > 0) && (flag == 1))
	{
		flag = 0;
		for (j = 0; j < m; j++)
			if (seletedCard[j] > seletedCard[j + 1])
			{
				flag = 1;
				temp = seletedCard[j];
				seletedCard[j] = seletedCard[j + 1];
				seletedCard[j + 1] = temp;
			}
		m--;
	}

	int num_max, num_t, data_t;
	int tempList[20], num_tempList = 0;
	int num_s = num_seleted;
	while (num_s)
	{
		num_max = 0;
		num_t = 1;
		data_t = 0;
		for (int i = 0; i < num_seleted; i++)
		{
			if (seletedCard[i] == -1) continue;
			if (((seletedCard[i + 1] / 4) == (seletedCard[i] / 4)) && seletedCard[i + 1] != -1)
				num_t++;
			else
			{
				if (num_t > num_max)
				{
					num_max = num_t;
					data_t = i - num_max + 1;
				}
				num_t = 1;
			}
		}
		for (int i = 0; i < num_max; i++)
		{
			tempList[num_tempList] = seletedCard[data_t];
			seletedCard[data_t] = -1;
			num_s--;
			num_tempList++;
			data_t++;
		}
	}
	for (int i = 0; i < num_tempList; i++)
		seletedCard[i] = tempList[i];
}

bool OLPukeManager::IsFeafible()
{
	sort_seleted();
	PukeType desk_type = checkType(deskCard, num_desk);
	if (num_desk > 0 && checkType(deskCard, num_desk) == illegal)
		desk_type = rocket;
	PukeType seleted_type = checkType(seletedCard, num_seleted);
	if (seleted_type == illegal)//不合法
		return false;
	else //合法
	{
		if (desk_type == illegal) //第一个出牌
			return true;
		else if (seleted_type == rocket)
			return true;
		else if (desk_type == rocket)
			return false;
		else if (desk_type == bomb)
		{
			if (seleted_type != bomb)
				return false;
			else
			{
				if (seletedCard[0] > deskCard[0])
					return true;
				else
					return false;
			}
		}
		else if (seleted_type == bomb)
		{
			if (desk_type != bomb)
				return true;
			else
			{
				if (seletedCard[0] > deskCard[0])
					return true;
				else
					return false;
			}
		}
		else
		{
			if (desk_type != seleted_type)
				return false;
			else
			{
				if (seletedCard[0] > deskCard[0])
					return true;
				else
					return false;
			}
		}
	}
}

bool OLPukeManager::handSeletedCard()
{
	if (IsFeafible())
	{
		::pt::DaChuDec nwe;
		nwe.dec = CHU;
		for (int i = 0; i < num_seleted; i++)
			nwe.cards.push_back(seletedCard[i]);
		::sf::Packet packet;
		packet << static_cast<int>(nwe.type()) << nwe;
		if (connector.sendNetworkEvent(packet))
		{
			for (int i = 0; i < 20; i++)
				seletedCard[i] = -1;
			num_seleted = 0;
			return true;
		}
		else
		{
			::std::cout << "disconnected.......\n";
			return false;
		}
	}
	else
		return false;
}