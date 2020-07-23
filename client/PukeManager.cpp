#include "PukeManager.h"

PukeManager::PukeManager()
{
	puke_dt_x = 0;
	puke_dt_e = 0;
	puke_chupai_lx = 640;
	puke_chupai_dx = 50;
	num_desk = 0;
	num_seleted = 0;
	num_temp = 54;
	this->human = NULL;
	this->ai_1 = NULL;
	this->ai_2 = NULL;
	for (int i = 0; i < 3; i++)
		dizhuCard[i] = -1;
	for (int i = 0; i < 54; i++)
	{
		temp[i] = 0;
		dealTemp[i] = i;
		puke_order[i] = 0;
	}
	for (int i = 0; i < 21; i++)
	{
		deskCard[i] = -1;
		seletedCard[i] = -1;
	}
	isGunCharm = false;
}

void PukeManager::clearAll()
{
	(*ai_1).sid = NO;
	(*ai_2).sid = NO;
	(*human).sid = NO;
	(*ai_1).isMyTime = false;
	(*ai_2).isMyTime = false;
	(*human).isMyTime = false;
	(*ai_1).isWin = false;
	(*ai_2).isWin = false;
	(*human).isWin = false;
	(*ai_1).isCallingDizhu = false;
	(*ai_2).isCallingDizhu = false;
	(*human).isCallingDizhu = false;
	(*ai_1).dec = NOT;
	(*ai_2).dec = NOT;
	(*human).dec = NOT;
	(*ai_1).num_card = 0;
	(*ai_2).num_card = 0;
	(*human).num_card = 0;
	(*ai_1).s_call = -1;
	(*ai_2).s_call = -1;
	(*human).s_call = -1;
	for (int i = 0; i < 20; i++)
	{
		(*ai_1).hand_card[i] = -1;
		(*ai_2).hand_card[i] = -1;
		(*human).hand_card[i] = -1;
	}
	puke_dt_x = 0;
	puke_dt_e = 0;
	puke_chupai_lx = 640;
	puke_chupai_dx = 50;
	num_desk = 0;
	num_seleted = 0;
	num_temp = 54;
	for (int i = 0; i < 3; i++)
		dizhuCard[i] = -1;
	for (int i = 0; i < 54; i++)
	{
		temp[i] = 0;
		dealTemp[i] = i;
		puke_order[i] = 0;
	}
	for (int i = 0; i < 21; i++)
	{
		deskCard[i] = -1;
		seletedCard[i] = -1;
	}
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			puke.c[i][j].dt_e = 0;
			puke.c[i][j].isDeleted = false;
			puke.c[i][j].isOnDesk = false;
			puke.c[i][j].isOnTop = false;
			puke.c[i][j].isPressed = false;
			puke.c[i][j].isSeleted = false;
			puke.c[i][j].sprite.setPosition(-100, -100);
		}
	}
	isGunCharm = false;
	gunCharm.clear();
}

void PukeManager::deal()
{
	srand(time(0));
	if (!isRhythm)
	{
		if (human->num_card == 17)
		{
			for (int i = 0; i < (*human).num_card; i++)
				dealTemp[human->hand_card[i]] = -1;
		}
		int k;
		while (true)
		{
			k = rand() % 54;
			if (dealTemp[k] != -1)
				break;
		}
		if ((*human).num_card < 17)
		{
			(*human).addCard(dealTemp[k]);
			dealTemp[k] = -1;
		}

		while (true)
		{
			k = rand() % 54;
			if (dealTemp[k] != -1)
				break;
		}
		if ((*ai_1).num_card < 17)
		{
			(*ai_1).addCard(dealTemp[k]);
			dealTemp[k] = -1;
		}

		while (true)
		{
			k = rand() % 54;
			if (dealTemp[k] != -1)
				break;
		}
		if ((*ai_2).num_card < 17)
		{
			(*ai_2).addCard(dealTemp[k]);
			dealTemp[k] = -1;
		}
		num_temp -= 3;
	}
	else
	{
		int k, l;
		rhythmCharm.update();
		for (int i = 0; i < 54; i++)
		{
			if (rhythmCharm.puke_now[i] == 1)
			{
				k = puke_order[i] / 4;
				l = puke_order[i] % 4;
				puke.c[k][l].sprite.setPosition(rhythmCharm.puke_pos[i].x, rhythmCharm.puke_pos[i].y);
				puke.c[k][l].sprite.setScale(0.6, 0.6);
			}
		}
	}
}

void PukeManager::getCard_rhythm(int n)
{
	for (int i = 0; i < 54; i++)
	{
		if (rhythmCharm.puke_now[i] == 0)
			continue;
		if (rhythmCharm.puke_pos[i].y > 500 && rhythmCharm.puke_pos[i].y < 600 && rhythmCharm.puke_pos[i].x == n * 100 + 440)
		{
			(*human).addCard(puke_order[i]);
			rhythmCharm.puke_now[i] = 0;
			rhythmCharm.speed += 0.5;
		}
	}
}

void PukeManager::deal_dizhuCard()
{
	int num_d = 0;
	for (int i = 0; i < 54; i++)
	{
		if (dealTemp[i] != -1)
		{
			dizhuCard[num_d++] = dealTemp[i];
			sDizhuCard[num_d - 1].sprite = puke.c[dealTemp[i] / 4][dealTemp[i] % 4].sprite;
		}
	}
	if ((*human).sid == DIZHU)
	{
		(*human).addCard(dizhuCard[0]);
		(*human).addCard(dizhuCard[1]);
		(*human).addCard(dizhuCard[2]);
		(*human).isMyTime = true;
	}
	else if ((*ai_1).sid == DIZHU)
	{
		(*ai_1).addCard(dizhuCard[0]);
		(*ai_1).addCard(dizhuCard[1]);
		(*ai_1).addCard(dizhuCard[2]);
		(*ai_1).isMyTime = true;
	}
	else if ((*ai_2).sid == DIZHU)
	{
		(*ai_2).addCard(dizhuCard[0]);
		(*ai_2).addCard(dizhuCard[1]);
		(*ai_2).addCard(dizhuCard[2]);
		(*ai_2).isMyTime = true;
	}
}

void PukeManager::Start()
{
	//gunCharm.start();
	rhythmCharm.start();
	clearAll();
	int m = 0, k;
	while (m < 54)
	{
		while (true)
		{
			k = rand() % 54;
			if (temp[k] != -1)
				break;
		}
		puke_order[m++] = k;
		temp[k] = -1;
	}
}

void PukeManager::update()
{
	for (int i = num_desk; i < 20; i++)
		deskCard[i] = -1;
	puke_dt_e = 55 - (*human).num_card;
	puke_dt_x = 620 - (*human).num_card * puke_dt_e / 2;

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

	puke_chupai_dx = 50 - num_desk;
	puke_chupai_lx = 620 - num_desk * puke_chupai_dx / 2;

	puke.c[(*human).hand_card[(*human).num_card - 1] / 4][(*human).hand_card[(*human).num_card - 1] % 4].isOnTop = true;

	if (!isRhythm)
	{
		for (int i = 0; i < (*human).num_card; i++)
		{
			int k = (*human).hand_card[i] / 4;
			int l = (*human).hand_card[i] % 4;
			int posY;
			if (puke.c[k][l].isSeleted)
				posY = 470;
			else
				posY = 520;
			puke.c[k][l].sprite.setPosition(puke_dt_x + i * puke_dt_e, posY);
		}
	}

	if (num_desk == 0)
		isGunCharm = false;

	int k = 0;
	for (int i = 0; i < num_desk; i++)
	{
		if (isGunCharm)
			gunCharm.updata();
		else
		{
			gunCharm.puke_pos[i].x = puke_chupai_lx + (k++) * puke_chupai_dx;
			gunCharm.puke_pos[i].y = 280;
			gunCharm.puke_scale[i] = 0.5;
			gunCharm.num = num_desk;
		}
		puke.c[deskCard[i] / 4][deskCard[i] % 4].sprite.setPosition(gunCharm.puke_pos[i].x, gunCharm.puke_pos[i].y);
		puke.c[deskCard[i] / 4][deskCard[i] % 4].sprite.setScale(gunCharm.puke_scale[i], gunCharm.puke_scale[i]);
		puke.c[deskCard[i] / 4][deskCard[i] % 4].isOnDesk = true;
	}

	if (((*human).isMyTime && (*human).dec == NOT && (*ai_1).dec == PASS && (*ai_2).dec == PASS) ||
		((*ai_1).isMyTime && (*ai_1).dec == NOT && (*human).dec == PASS && (*ai_2).dec == PASS) ||
		((*ai_2).isMyTime && (*ai_2).dec == NOT && (*ai_1).dec == PASS && (*human).dec == PASS))
	{
		for (int i = 0; i < num_desk; i++)
			deskCard[i] = -1;
		num_desk = 0;
	}

	for (int i = 0; i < 14; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (puke.c[i][j].isDeleted || puke.c[i][j].isOnDesk)
			{
				for (int k = 0; k < 3; k++)
				{
					if (dizhuCard[k] == i * 4 + j)
						sDizhuCard[k].sprite.setColor(Color(125, 125, 125, 125));
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////////test///////////////////////////////////////////////////////////////////////////////
	/*srand(time(0));
	if (rand() % 15 == 0)
	{
		dfs_card.clear();
		dfs_card.add_to_dfs((*human).hand_card, (*human).num_card);
		dfs_card.dfs(dfs_card.a, 0);
		std::cout << dfs_card.ans << std::endl;
	}*/
}

void PukeManager::removeFromDesk(int i)
{
	for (int j = i; j < num_desk - 1; j++)
	{
		deskCard[j] = deskCard[j + 1];
		gunCharm.puke_pos[j].x = gunCharm.puke_pos[j + 1].x;
		gunCharm.puke_pos[j].y = gunCharm.puke_pos[j + 1].y;
		gunCharm.puke_scale[j] = gunCharm.puke_scale[j + 1];
	}
	num_desk--;
	gunCharm.num--;
}

bool PukeManager::findWithType(PukeType type, int low, int* source_card, int* result_card)
{
	int temp_hand_card[20];
	memset(temp_hand_card, -1, sizeof(temp_hand_card));
	int num = 0;
	for (int i = 0; i < 20; i++)
		if (source_card[i] >= 0 && source_card[i] <= 53)
			temp_hand_card[num++] = source_card[i];
	int m3 = 0, m2 = 0, m1 = 0;
	for (int i = 0; i < num; i++)
	{
		if (temp_hand_card[i] / 4 < low / 4)
			continue;
		if (type == four_single)
		{
			if (temp_hand_card[i] / 4 == temp_hand_card[i + 3] / 4 && temp_hand_card[i + 3] != -1)
			{
				int temp_result[20];
				memset(temp_result, -1, sizeof(temp_result));
				int num_tr = 0;
				for (int j = 0; j < 4; j++)
				{
					temp_result[num_tr++] = temp_hand_card[i + j];
					temp_hand_card[i + j] = -1;
				}
				int t1[1] = { -1 };
				if (findWithType(single, 0, temp_hand_card, t1))
				{
					temp_result[num_tr++] = t1[0];
					for (int j = 0; j < num; j++)
					{
						if (temp_hand_card[j] == t1[0])
						{
							temp_hand_card[j] = -1;
							break;
						}
					}
					t1[0] = -1;
					if (findWithType(single, 0, temp_hand_card, t1))
					{
						temp_result[num_tr++] = t1[0];
						for (int kk = 0; kk < num_tr; kk++)
							result_card[kk] = temp_result[kk];
						return true;
					}
					else
						return false;
				}
				else
					return false;
			}
			else
				continue;
		}
		else if (type == four_couple)
		{
			if (temp_hand_card[i] / 4 == temp_hand_card[i + 3] / 4 && temp_hand_card[i + 3] != -1)
			{
				int temp_result[20] = { -1 };
				memset(temp_result, -1, sizeof(temp_result));
				int num_tr = 0;
				for (int j = 0; j < 4; j++)
				{
					temp_result[num_tr++] = temp_hand_card[i + j];
					temp_hand_card[i + j] = -1;
				}
				int t1[2] = { -1 };
				if (findWithType(couple, 0, temp_hand_card, t1))
				{
					temp_result[num_tr++] = t1[0];
					temp_result[num_tr++] = t1[1];
					for (int j = 0; j < num; j++)
					{
						if (temp_hand_card[j] == t1[0] || temp_hand_card[j] == t1[1])
							temp_hand_card[j] = -1;
					}
					t1[0] = -1, t1[1] = -1;
					if (findWithType(single, 0, temp_hand_card, t1))
					{
						temp_result[num_tr++] = t1[0];
						temp_result[num_tr++] = t1[1];
						for (int kk = 0; kk < num_tr; kk++)
							result_card[kk] = temp_result[kk];
						return true;
					}
					else
						return false;
				}
				else
					return false;
			}
			else
				continue;
		}
		else
		{
			switch (type)
			{
			case illegal:
				return false;
				break;
			case single:
				m1 = 1;
				break;
			case single_list5:
				m1 = 5;
				break;
			case single_list6:
				m1 = 6;
				break;
			case single_list7:
				m1 = 7;
				break;
			case single_list8:
				m1 = 8;
				break;
			case single_list9:
				m1 = 9;
				break;
			case single_list10:
				m1 = 10;
				break;
			case single_list11:
				m1 = 11;
				break;
			case single_list12:
				m1 = 12;
				break;
			case couple:
				m2 = 1;
				break;
			case couple_list3:
				m2 = 3;
				break;
			case couple_list4:
				m2 = 4;
				break;
			case couple_list5:
				m2 = 5;
				break;
			case couple_list6:
				m2 = 6;
				break;
			case couple_list7:
				m2 = 7;
				break;
			case couple_list8:
				m2 = 8;
				break;
			case couple_list9:
				m2 = 9;
				break;
			case couple_list10:
				m2 = 10;
				break;
			case three:
				m3 = 1;
				break;
			case three_single:
				m3 = 1;
				break;
			case three_couple:
				m3 = 1;
				break;
			case three_list2:
				m3 = 2;
				break;
			case three_list3:
				m3 = 3;
				break;
			case three_list4:
				m3 = 4;
				break;
			case three_list5:
				m3 = 5;
				break;
			case three_list6:
				m3 = 6;
				break;
			case three_list2_single:
				m3 = 2;
				break;
			case three_list3_single:
				m3 = 3;
				break;
			case three_list4_single:
				m3 = 4;
				break;
			case three_list5_single:
				m3 = 5;
				break;
			case three_list2_couple:
				m3 = 2;
				break;
			case three_list3_couple:
				m3 = 3;
				break;
			case three_list4_couple:
				m3 = 4;
				break;
			default:
				break;
			}
			if (m3)
			{
				for (int j = 0; j < num; j++)
				{
					if (temp_hand_card[j] / 4 < low / 4)
						continue;
					int t = 0;
					for (int k = 0; k < m3; k++)
						if (j + k * 3 + 2 < 20)
							if (temp_hand_card[j + k * 3] / 4 == temp_hand_card[j + k * 3 + 2] / 4)
								t++;
					if (t == m3)
					{
						if (type == three_list6)
						{
							for (int l = 0; l < m3 * 3; l++)
								result_card[l] = temp_hand_card[j + l];
							return true;
						}
						else if (type == three_list5_single)
						{
							for (int l = 0; l < num; l++)
								result_card[l] = temp_hand_card[l];
							return true;
						}
						else if (type == three_list4_couple)
						{
							int temp_re[20] = { -1 }, num_tr = 0;
							for (int l = 0; l < 12; l++)
							{
								temp_re[num_tr++] = temp_hand_card[j + l];
								temp_hand_card[j + l] = -1;
							}
							for (int jj = 0; jj < 4; jj++)
							{
								int t[2] = { -1 };
								if (findWithType(couple, 0, temp_hand_card, t))
								{
									for (int l = 0; l < 2; l++)
									{
										temp_re[num_tr++] = t[l];
										for (int kk = 0; kk < num; kk++)
											if (temp_hand_card[kk] == t[l])
												temp_hand_card[kk] = -1;
									}
								}
								else
									return false;
							}
							for (int jj = 0; jj < num_tr; jj++)
								result_card[jj] = temp_re[jj];
							return true;
						}
						else if (type == three_list4_single)
						{
							int temp_re[16] = { -1 }, num_tr = 0;
							for (int l = 0; l < 12; l++)
							{
								temp_re[num_tr++] = temp_hand_card[j + l];
								temp_hand_card[j + l] = -1;
							}
							for (int jj = 0; jj < 4; jj++)
							{
								int t[1] = { -1 };
								if (findWithType(couple, 0, temp_hand_card, t))
								{
									temp_re[num_tr++] = t[0];
									for (int kk = 0; kk < 20; kk++)
										if (temp_hand_card[kk] == t[0])
											temp_hand_card[kk] = -1;
								}
								else
									return false;
							}
							for (int jj = 0; jj < num_tr; jj++)
								result_card[jj] = temp_re[jj];
							return true;
						}
						else if (type == three_list3_couple)
						{
							int temp_re[15], num_tr = 0;
							memset(temp_re, -1, sizeof(temp_re));
							for (int l = 0; l < 9; l++)
							{
								temp_re[num_tr++] = temp_hand_card[j + l];
								temp_hand_card[j + l] = -1;
							}
							for (int jj = 0; jj < 3; jj++)
							{
								int t[2] = { -1 };
								if (findWithType(couple, 0, temp_hand_card, t))
								{
									for (int l = 0; l < 2; l++)
									{
										temp_re[num_tr++] = t[l];
										for (int kk = 0; kk < num; kk++)
											if (temp_hand_card[kk] == t[l])
												temp_hand_card[kk] = -1;
									}
								}
								else
									return false;
							}
							for (int jj = 0; jj < num_tr; jj++)
								result_card[jj] = temp_re[jj];
							return true;
						}
						else if (type == three_list3_single)
						{
							int temp_re[12], num_tr = 0;
							memset(temp_re, -1, sizeof(temp_re));
							for (int l = 0; l < 9; l++)
							{
								temp_re[num_tr++] = temp_hand_card[j + l];
								temp_hand_card[j + l] = -1;
							}
							for (int jj = 0; jj < 3; jj++)
							{
								int t[1] = { -1 };
								if (findWithType(couple, 0, temp_hand_card, t))
								{
									temp_re[num_tr++] = t[0];
									for (int kk = 0; kk < num; kk++)
										if (temp_hand_card[kk] == t[0])
											temp_hand_card[kk] = -1;
								}
								else
									return false;
							}
							for (int jj = 0; jj < num_tr; jj++)
								result_card[jj] = temp_re[jj];
							return true;
						}
						else if (type == three_list2_couple)
						{
							int temp_re[10] = { -1 }, num_tr = 0;
							for (int l = 0; l < 6; l++)
							{
								temp_re[num_tr++] = temp_hand_card[j + l];
								temp_hand_card[j + l] = -1;
							}
							for (int jj = 0; jj < 2; jj++)
							{
								int t[2] = { -1 };
								if (findWithType(couple, 0, temp_hand_card, t))
								{
									for (int l = 0; l < 2; l++)
									{
										temp_re[num_tr++] = t[l];
										for (int kk = 0; kk < num; kk++)
											if (temp_hand_card[kk] == t[l])
												temp_hand_card[kk] = -1;
									}
								}
								else
									return false;
							}
							for (int jj = 0; jj < num_tr; jj++)
								result_card[jj] = temp_re[jj];
							return true;
						}
						else if (type == three_list2_single)
						{
							int temp_re[8] = { -1 }, num_tr = 0;
							for (int l = 0; l < 6; l++)
							{
								temp_re[num_tr++] = temp_hand_card[j + l];
								temp_hand_card[j + l] = -1;
							}
							for (int jj = 0; jj < 2; jj++)
							{
								int t[1] = { -1 };
								if (findWithType(couple, 0, temp_hand_card, t))
								{
									temp_re[num_tr++] = t[0];
									for (int kk = 0; kk < num; kk++)
										if (temp_hand_card[kk] == t[0])
											temp_hand_card[kk] = -1;
								}
								else
									return false;
							}
							for (int jj = 0; jj < num_tr; jj++)
								result_card[jj] = temp_re[jj];
							return true;
						}
						else if (type == three_couple)
						{
							int temp_re[5] = { -1 }, num_tr = 0;
							for (int l = 0; l < 3; l++)
							{
								temp_re[num_tr++] = temp_hand_card[j + l];
								temp_hand_card[j + l] = -1;
							}
							int t[2] = { -1 };
							if (findWithType(couple, 0, temp_hand_card, t))
							{
								for (int l = 0; l < 2; l++)
								{
									temp_re[num_tr++] = t[l];
									for (int kk = 0; kk < num; kk++)
										if (temp_hand_card[kk] == t[l])
											temp_hand_card[kk] = -1;
								}
							}
							else
								return false;
							for (int jj = 0; jj < num_tr; jj++)
								result_card[jj] = temp_re[jj];
							return true;
						}
						else if (type == three_single)
						{
							int temp_re[4], num_tr = 0;
							memset(temp_re, -1, sizeof(temp_re));
							for (int l = 0; l < 3; l++)
							{
								temp_re[num_tr++] = temp_hand_card[j + l];
								temp_hand_card[j + l] = -1;
							}
							int t[1] = { -1 };
							if (findWithType(couple, 0, temp_hand_card, t))
							{
								temp_re[num_tr++] = t[0];
								for (int kk = 0; kk < num; kk++)
									if (temp_hand_card[kk] == t[0])
										temp_hand_card[kk] = -1;
							}
							else
								return false;
							for (int jj = 0; jj < num_tr; jj++)
								result_card[jj] = temp_re[jj];
							return true;
						}
						else if (type == three)
						{
							int temp_re[3], num_tr = 0;
							memset(temp_re, -1, sizeof(temp_re));
							for (int l = 0; l < 3; l++)
								result_card[l] = temp_re[l];
							return true;
						}
					}
					else
						continue;
				}
				return false;
			}
			else if (m2)
			{
				for (int j = 0; j < num; j++)
				{
					if (temp_hand_card[j] / 4 < low / 4)
						continue;
					int t = 0;
					if (m2 == 1)
					{
						int temp_re[2];
						memset(temp_re, -1, sizeof(temp_re));
						for (int ii = j; ii < num; ii++)
						{
							if (temp_hand_card[ii] / 4 == temp_hand_card[ii + 1] / 4)
							{
								int num_min = 100;
								int temp2[20], num_t2 = 0;
								for (int jj = 0; jj < num; jj++)
									if (jj != ii && jj != ii + 1)
										temp2[num_t2++] = temp_hand_card[jj];
								dfs_card.clear();
								dfs_card.add_to_dfs(temp2, num_t2);
								dfs_card.dfs(dfs_card.a, 0);
								if (dfs_card.ans < num_min)
								{
									temp_re[0] = temp_hand_card[ii];
									temp_re[1] = temp_hand_card[ii + 1];
									num_min = dfs_card.ans;
								}
							}
						}
						if (temp_re[0] != -1)
						{
							result_card[0] = temp_re[0];
							result_card[1] = temp_re[1];
							return true;
						}
						else
							return false;
					}
					else
					{
						int tem = -1;
						for (int ii = j; ii < num; ii++)
						{
							if (tem == temp_hand_card[ii] / 4)
								continue;
							if (temp_hand_card[ii] / 4 == temp_hand_card[ii + 1] / 4)
							{
								if (tem != -1)
								{
									if (tem + 1 == temp_hand_card[ii] / 4)
									{
										t++;
										ii++;
										tem = temp_hand_card[ii] / 4;
									}
									else
									{
										t = 0;
										tem = -1;
									}
								}
								else
								{
									t++;
									ii++;
									tem = temp_hand_card[ii] / 4;
								}
							}
							if (t == m2)
							{
								int num_re = 0;
								for (int jj = 0; jj < m2; i++)
								{
									for (int kk = 0; kk < num; kk++)
									{
										if (temp_hand_card[kk] / 4 == tem - 2 * jj)
											result_card[num_re++] = temp_hand_card[kk];
									}
								}
								return true;
							}
						}
						return false;
					}
				}
				return false;
			}
			else if (m1)
			{
				for (int j = 0; j < num; j++)
				{
					if (temp_hand_card[j] / 4 < low / 4)
						continue;
					int t = 0;
					if (m1 == 1)
					{
						int tem = -1;
						for (int ii = j; ii < num; ii++)
						{
							int num_min = 100;
							int temp2[20], num_t2 = 0;
							memset(temp2, -1, sizeof(temp2));
							for (int jj = 0; jj < num; jj++)
								if (jj != ii)
									temp2[num_t2++] = temp_hand_card[jj];
							dfs_card.clear();
							dfs_card.add_to_dfs(temp2, num_t2);
							dfs_card.dfs(dfs_card.a, 0);
							if (dfs_card.ans < num_min)
							{
								tem = temp_hand_card[ii];
								num_min = dfs_card.ans;
							}
						}
						if (tem != -1)
						{
							result_card[0] = tem;
							return true;
						}
						else
							return false;
					}
					else
					{
						int tem = -1;
						for (int ii = j; ii < num; ii++)
						{
							if (tem == temp_hand_card[ii] / 4)
								continue;
							if (tem != -1)
							{
								if (tem + 1 == temp_hand_card[ii] / 4 && temp_hand_card[ii] < 48)
								{
									t++;
									tem = temp_hand_card[ii] / 4;
								}
								else
								{
									t = 0;
									tem = -1;
								}
							}
							else
							{
								t++;
								tem = temp_hand_card[ii] / 4;
							}
							if (t == m1)
							{
								int num_re = 0;
								for (int jj = 0; jj < m1; jj++)
								{
									for (int kk = num - 1; kk >= 0; kk--)
									{
										if (temp_hand_card[kk] / 4 == tem - jj)
										{
											result_card[num_re++] = temp_hand_card[kk];
											temp_hand_card[kk] = -1;
											break;
										}
									}
								}
								return true;
							}
						}
						return false;
					}
				}
				return false;
			}
			else
				return false;
		}
	}
	return false;
}

void PukeManager::autoSeleteCard(AI* ai, PukeType& type)
{
	(*ai).dec = NOT;
	srand(time(0));
	if (!(*ai).clock_chupai.isRun)
		(*ai).clock_chupai.start();
	(*ai).clock_chupai.update();
	(*ai).tDaojishi.setString(std::to_string(30 - (*ai).clock_chupai.second));
	if ((*ai).clock_chupai.second >= 1)
	{
		int templist[14] = { 0 };
		for (int i = 0; i < (*ai).num_card; i++)
			templist[(*ai).hand_card[i] / 4]++;
		dfs_card.clear();
		dfs_card.add_to_dfs((*ai).hand_card, (*ai).num_card);
		dfs_card.dfs(dfs_card.a, 0);
		dfs_card.getFirst();
		int num_t = dfs_card.ans;
		if (num_desk == 0)//控手主动出牌
		{
			for (int i = 0; i < 14; i++)
			{
				if (dfs_card.first[i])
				{
					for (int j = 0; j < (*ai).num_card; j++)
					{
						if ((*ai).hand_card[j] / 4 == i)
						{
							for (int k = 0; k < dfs_card.first[i]; k++)
								addToJudge(i, (*ai).hand_card[j + k] % 4);
							break;
						}
					}
				}
			}
			sort_seleted();
			PukeType seleted_type = checkType(seletedCard, num_seleted);
			if (seleted_type != illegal) //合法
			{
				for (int i = 0; i < num_seleted; i++)
				{
					deskCard[num_desk++] = seletedCard[i];
					(*ai).removeCard(seletedCard[i]);
					seletedCard[i] = -1;
				}
				num_seleted = 0;
			}
			else
			{
				for (int i = 0; i < num_seleted; i++)
					seletedCard[i] = -1;
				num_seleted = 0;
				bool flag = false;
				for (int i = 0; i < 14; i++)
				{
					if (dfs_card.first[i] == 3)
					{
						for (int j = 0; j < (*ai).num_card; j++)
						{
							if ((*ai).hand_card[j] / 4 == i)
							{
								for (int k = 0; k < 3; k++)
								{
									deskCard[num_desk++] = (*ai).hand_card[j];
									(*ai).removeCard((*ai).hand_card[j]);
								}
								break;
							}
						}
						flag = true;
						for (int j = 0; j < 14; j++)
						{
							if (dfs_card.first[j] == 1 || dfs_card.first[j] == 2)
							{
								for (int k = 0; k < (*ai).num_card; k++)
								{
									if ((*ai).hand_card[k] / 4 == j)
									{
										for (int l = 0; l < dfs_card.first[j]; l++)
										{
											deskCard[num_desk++] = (*ai).hand_card[k];
											(*ai).removeCard((*ai).hand_card[k]);
										}
										break;
									}
								}
								break;
							}
						}
						break;
					}
				}
				if (!flag)
				{
					for (int i = 0; i < 14; i++)
					{
						if (dfs_card.first[i])
						{
							for (int j = 0; j < (*ai).num_card; j++)
							{
								if ((*ai).hand_card[j] / 4 == i)
								{
									for (int k = 0; k < dfs_card.first[i]; k++)
									{
										deskCard[num_desk++] = (*ai).hand_card[j];
										(*ai).removeCard((*ai).hand_card[j]);
									}
									break;
								}
							}
							break;
						}
					}
				}
			}
			(*ai).isMyTime = false;
			(*ai).dec = CHU;
		}
		else//被动
		{
			bool isOk = false;
			PukeType desk_type = checkType(deskCard, num_desk);
			if (num_desk > 0 && checkType(deskCard, num_desk) == illegal)
				desk_type = rocket;
			if (desk_type == rocket)
				isOk = false;
			else if (num_t <= 3)
			{
				if ((*ai).num_card >= 2 && (*ai).hand_card[(*ai).num_card - 2] == 52 && (*ai).hand_card[(*ai).num_card - 1] == 53)
				{
					seletedCard[0] = 52;
					seletedCard[1] = 53;
					num_seleted = 2;
					isOk = true;
				}
				else
				{
					for (int i = 13; i >= 0; i--)
					{
						if (templist[i] == 4 && i * 4 > deskCard[0])
						{
							isOk = true;
							for (int j = 0; j < 4; j++)
							{
								seletedCard[num_seleted++] = i * 4 + j;
								(*ai).removeCard(i * 4 + j);
							}
						}
					}
				}
			}
			if (isOk == false)
			{
				int selist[20];
				memset(selist, -1, sizeof(selist));
				if (findWithType(desk_type, (deskCard[0] / 4 + 1) * 4, (*ai).hand_card, selist))
				{
					for (int i = 0; i < 20; i++)
						seletedCard[i] = -1;
					num_seleted = 0;
					for (int i = 0; selist[i] != -1; i++)
						seletedCard[num_seleted++] = selist[i];
					isOk = true;
				}
				else
					isOk = false;
			}
			if (isOk)
			{
				sort_seleted();
				for (int i = 0; i < num_desk; i++)
					deskCard[i] = -1;
				num_desk = 0;
				for (int i = 0; i < num_seleted; i++)
				{
					deskCard[num_desk++] = seletedCard[i];
					(*ai).removeCard(seletedCard[i]);
					seletedCard[i] = -1;
				}
				num_seleted = 0;
				(*ai).isMyTime = false;
				(*ai).dec = CHU;
				type = checkType(deskCard, num_desk);
			}
			else
			{
				(*ai).isMyTime = false;
				(*ai).dec = PASS;
				type = illegal;
			}
		}
		(*ai).clock_chupai.stop();
	}
	else return;
}

void PukeManager::addToJudge(int i, int j)
{
	seletedCard[num_seleted++] = 4 * i + j;
}

void PukeManager::sort_seleted()
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

PukeType PukeManager::checkType(int* card, int num)
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

void PukeManager::JudgeCard(Player& human, PukeType& type)
{
	bool isOk = false;
	sort_seleted();
	PukeType desk_type = checkType(deskCard, num_desk);
	if (num_desk > 0 && checkType(deskCard, num_desk) == illegal)
		desk_type = rocket;
	PukeType seleted_type = checkType(seletedCard, num_seleted);
	if (seleted_type == illegal)//不合法
		isOk = false;
	else //合法
	{
		if (desk_type == illegal) //第一个出牌
			isOk = true;
		else if (seleted_type == rocket)
			isOk = true;
		else if (desk_type == rocket)
			isOk = false;
		else if (desk_type == bomb)
		{
			if (seleted_type != bomb)
				isOk = false;
			else
			{
				if (seletedCard[0] > deskCard[0])
					isOk = true;
				else
					isOk = false;
			}
		}
		else if (seleted_type == bomb)
		{
			if (desk_type != bomb)
				isOk = true;
			else
			{
				if (seletedCard[0] > deskCard[0])
					isOk = true;
				else
					isOk = false;
			}
		}
		else
		{
			if (desk_type != seleted_type)
				isOk = false;
			else
			{
				if (seletedCard[0] > deskCard[0])
					isOk = true;
				else
					isOk = false;
			}
		}
	}
	//isOk = true;
	if (isOk)
	{
		for (int i = 0; i < num_desk; i++)
		{
			puke.c[deskCard[i] / 4][deskCard[i] % 4].isOnDesk = false;
			puke.c[deskCard[i] / 4][deskCard[i] % 4].isDeleted = true;
			deskCard[i] = -1;
		}
		num_desk = 0;
		for (int i = 0; i < num_seleted; i++)
		{
			deskCard[num_desk++] = seletedCard[i];
			puke.c[seletedCard[i] / 4][seletedCard[i] % 4].isSeleted = false;
			human.removeCard(seletedCard[i]);
			seletedCard[i] = -1;
		}
		num_seleted = 0;
		human.isMyTime = false;
		human.dec = CHU;
		type = seleted_type;
	}
	else
	{
		for (int i = 0; i < num_seleted; i++)
		{
			puke.c[seletedCard[i] / 4][seletedCard[i] % 4].isSeleted = false;
			seletedCard[i] = -1;
		}
		num_seleted = 0;
		human.dec = NOT;
		type = illegal;
	}
}