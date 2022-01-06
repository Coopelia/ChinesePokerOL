#pragma once
#include"Def.h"

class Player//玩家类
{
public:
	SF sid;//身份
	DEC dec;//抉择状态
	::std::vector<int> hand_card;//手牌数据数组
	int s_call;//叫地主的分值
	bool isCallingDizhu;//是否在叫地主
	bool isMyTime;//是否轮到自己出牌
	bool isWin;
	bool isReady;
	Player();
	void addCard(int x);//添加x到手牌
	void removeCard(int x);//把x移除手牌
	void clear();
};