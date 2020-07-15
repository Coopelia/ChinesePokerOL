#include "Player.h"

User::User()
{
	app = NULL;
	//name = " ";
	sid = NO;
	dec = NOT;
	isMyTime = false;
	isWin = false;
	num_card = 0;
	s_call = -1;
	for (int i = 0; i < 20; i++)
	{
		hand_card[i] = -1;
	}
	tHead[0].loadFromFile("assets/image/game/房间/no.png");
	tHead[1].loadFromFile("assets/image/game/房间/dizhu.png");
	tHead[2].loadFromFile("assets/image/game/房间/nonmin.png");
	tCallNo.loadFromFile("assets/image/game/功能/斗地主/提示_不抢.png");
	tCallOne.loadFromFile("assets/image/game/功能/斗地主/提示_1.png");
	tCallTwo.loadFromFile("assets/image/game/功能/斗地主/提示_2.png");
	tCallThree.loadFromFile("assets/image/game/功能/斗地主/提示_3.png");
	tNoCard.loadFromFile("assets/image/game/功能/斗地主/提示_不出.png");
	tClock.loadFromFile("assets/image/game/功能/斗地主/jsq.png");
	sClock.setTexture(tClock);
	sHead.setTexture(tHead[0]);
	sNoCard.setTexture(tNoCard);
	font.loadFromFile("assets/fonts/fSimpleRound.ttf");
	tDaojishi.setFont(font);
	tDaojishi.setCharacterSize(32);
	tDaojishi.setFillColor(Color::Yellow);
	tNum_rest.setFont(font);
	tNum_rest.setString(std::to_string(num_card));
	tNum_rest.setCharacterSize(24);
	tNum_rest.setFillColor(Color::Yellow);
}

void User::Initial(RenderWindow* app)
{
	this->app = app;
}

void User::update()
{
	switch (sid)
	{
	case NO:
		//name = " ";
		sHead.setTexture(tHead[0]);
		break;
	case DIZHU:
		//name = L"地主";
		sHead.setTexture(tHead[1]);
		break;
	case NONM:
		//name = L"农民";
		sHead.setTexture(tHead[2]);
		break;
	default:
		break;
	}
	if (s_call != -1)
	{
		sCall.setColor(Color(255, 255, 255, 255));
		if (s_call == 0)
			sCall.setTexture(tCallNo);
		else if (s_call == 1)
			sCall.setTexture(tCallOne);
		else if (s_call == 2)
			sCall.setTexture(tCallTwo);
		else if (s_call == 3)
			sCall.setTexture(tCallThree);
	}
	else
		sCall.setColor(Color(0, 0, 0, 0));
	tNum_rest.setString(std::to_string(num_card));
	sort();
}

void User::sort()
{
	int m, j, flag;
	int temp;
	m = num_card - 1;
	flag = 1;
	while ((m > 0) && (flag == 1))
	{
		flag = 0;
		for (j = 0; j < m; j++)
			if (hand_card[j] > hand_card[j + 1])
			{
				flag = 1;
				temp = hand_card[j];
				hand_card[j] = hand_card[j + 1];
				hand_card[j + 1] = temp;
			}
		m--;
	}
}

void User::addCard(int x)
{
	hand_card[num_card++] = x;
}

void User::removeCard(int x)
{
	if (num_card)
	{
		int k = 0;
		for (int i = 0; i < num_card; i++)
		{
			if (hand_card[i] == x)
				k = i;
		}
		if (k < num_card - 1)
		{
			for (int i = k; i < num_card - 1; i++)
			{
				hand_card[i] = hand_card[i + 1];
			}
		}
		hand_card[num_card - 1] = -1;
		num_card--;
	}
}

void User::setPosition(int x, int y)
{
	sHead.setPosition(x, y);
}

void User::analyse_card(int* card, int num, int** list)
{
	int nt = 1, lm = 0;
	for (int i = 0; i < num; i++)
	{
		int k = card[i] / 4, l = card[i + 1] / 4;
		if (k == l && card[i + 1] != -1)
			nt++;
		else
		{
			if (list[0][0] == 0)
			{
				list[0][0] = nt;
				list[0][1]++;
			}
			else
			{
				if (list[lm][0] == nt)
					list[lm][1]++;
				else
				{
					list[++lm][0] = nt;
					list[lm][1]++;
				}
			}
			nt = 1;
		}
	}
}

void User::show()
{
	(*app).draw(sHead);
}

Player::Player()
{
	app = NULL;
	//name = " ";
	sid = NO;
	isMyTime = false;
	isCallingDizhu = false;
	num_card = 0;
	for (int i = 0; i < 20; i++)
	{
		hand_card[i] = -1;
	}
	bt_chupai.setTextrue("assets/image/game/功能/斗地主/go.png");
	bt_pass.setTextrue("assets/image/game/功能/斗地主/pass.png");
	bt_callThree.setTextrue("assets/image/game/功能/斗地主/three.png");
	bt_callTwo.setTextrue("assets/image/game/功能/斗地主/two.png");
	bt_callOne.setTextrue("assets/image/game/功能/斗地主/one.png");
	bt_callNo.setTextrue("assets/image/game/功能/斗地主/nocall.png");
	bt_chupai.setPosition(500, 400);
	bt_chupai.setScale(0.7, 0.7);
	bt_pass.setPosition(710, 400);
	bt_pass.setScale(0.7, 0.7);
	bt_callThree.setPosition(600, 480);
	bt_callTwo.setPosition(650, 480);
	bt_callOne.setPosition(700, 480);
	bt_callNo.setPosition(750, 480);
	tHead[0].loadFromFile("assets/image/game/房间/no.png");
	tHead[1].loadFromFile("assets/image/game/房间/dizhu.png");
	tHead[2].loadFromFile("assets/image/game/房间/nonmin.png");
	tMeg.loadFromFile("assets/image/game/功能/斗地主/底框.png");
	sMeg.setTexture(tMeg);
	sMeg.setPosition(0, 638);
	sHead.setTexture(tHead[0]);
	font.loadFromFile("assets/fonts/fSimpleRound.ttf");
	tNum_rest.setFont(font);
	tNum_rest.setString(std::to_string(num_card));
	tNum_rest.setCharacterSize(24);
	tNum_rest.setFillColor(Color::Yellow);
}

void Player::Initial(RenderWindow* app)
{
	this->app = app;
	this->bt_pass.app = app;
	this->bt_chupai.app = app;
	this->bt_callThree.app = app;
	this->bt_callTwo.app = app;
	this->bt_callOne.app = app;
	this->bt_callNo.app = app;
}

void Player::callDizhu(Event& e)
{
	if (bt_callThree.onClick(e))
		s_call = 3;
	else if (bt_callTwo.onClick(e))
		s_call = 2;
	else if (bt_callOne.onClick(e))
		s_call = 1;
	else if (bt_callNo.onClick(e))
		s_call = 0;
	else
		s_call = -1;
	if (s_call != -1)
		isCallingDizhu = false;
}

void Player::show()
{
	(*app).draw(sMeg);
	(*app).draw(sHead);
	if (isMyTime)
	{
		bt_chupai.show();
		bt_pass.show();
	}
	if (isCallingDizhu)
	{
		bt_callThree.show();
		bt_callTwo.show();
		bt_callOne.show();
		bt_callNo.show();
	}
}

AI::AI()
{
	app = NULL;
	//name = " ";
	sid = NO;
	isMyTime = false;
	num_card = 0;
	for (int i = 0; i < 20; i++)
	{
		hand_card[i] = -1;
	}
	tHead[0].loadFromFile("assets/image/game/房间/no.png");
	tHead[1].loadFromFile("assets/image/game/房间/dizhu.png");
	tHead[2].loadFromFile("assets/image/game/房间/nonmin.png");
	sHead.setTexture(tHead[0]);
	font.loadFromFile("assets/fonts/fSimpleRound.ttf");
	tNum_rest.setFont(font);
	tNum_rest.setString(std::to_string(num_card));
	tNum_rest.setCharacterSize(24);
	tNum_rest.setFillColor(Color::Yellow);
}

void AI::Initial(RenderWindow* app)
{
	this->app = app;
}
//当前手牌最少多少次可以出完来叫分，<=3次叫3分，再者<=5次叫二分，其次<=7次叫1分，否则不叫
void AI::callDizhu()
{
	srand(time(0));
	if (rand() % 10 != 0)
		s_call = -1;

	dfs_card.clear();
	dfs_card.add_to_dfs(hand_card, num_card);
	dfs_card.dfs(dfs_card.a, 0);
	int score = dfs_card.ans;

	if (!clock_callDizhu.isRun)
		clock_callDizhu.start();
	clock_callDizhu.update();
	if (clock_callDizhu.second >= 1)
	{
		clock_callDizhu.stop();
		if (score <= 3)
			s_call = 3;
		else if (score <= 5)
			s_call = 2;
		else if (score <= 7)
			s_call = 1;
		else
			s_call = 0;
		if (s_call != -1)
			isCallingDizhu = false;
	}
}

void AI::show()
{
	(*app).draw(sHead);
}