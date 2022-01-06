#include "Player.h"

Player::Player()
{
	sid = NO;
	dec = NOT;
	s_call = -1;
	isCallingDizhu = false;
	isMyTime = false;
	isWin = false;
	isReady = true;
}

void Player::addCard(int x)
{
	hand_card.push_back(x);
	::std::sort(hand_card.begin(), hand_card.end(), [](int a, int b)->bool {return a <= b; });
}

void Player::removeCard(int x)
{
	::std::vector<int>::iterator itr;
	for (itr = hand_card.begin(); itr !=hand_card.end();)
	{
		if ((*itr) == x)
		{
			hand_card.erase(itr);
			return;
		}
		else
			itr++;
	}
}

void Player::clear()
{
	sid = NO;
	dec = NOT;
	hand_card.clear();
	s_call = -1;
	isCallingDizhu = false;
	isMyTime = false;
	isWin = false;
}