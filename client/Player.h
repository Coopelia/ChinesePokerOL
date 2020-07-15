#pragma once
#include"Def.h"
#include"Button.h"
#include"MyClock.h"
#include"DFSCard.h"

struct  PlayerInfo
{
	bool isReady;
	bool isMyTime;
	int playerId;//0-2
};

class User//用户类
{
public:
	RenderWindow* app;
	SF sid;//身份
	DEC dec;//抉择状态
	int hand_card[20];//手牌数据数组
	int num_card;//手牌数量
	int s_call;//叫地主的分值
	bool isCallingDizhu;//是否在叫地主
	bool isMyTime;//是否轮到自己出牌
	bool isWin;
	MyClock clock_daojishi;//出牌倒计时
	Sprite sHead, sCall, sNoCard, sClock;
	Texture tHead[3], tCallOne, tCallTwo, tCallThree, tCallNo, tNoCard, tClock;
	Font font;
	Text tNum_rest, tDaojishi;
	User();
	void update();//每帧更新
	void sort();//排序手牌
	void addCard(int x);//添加x到手牌
	void removeCard(int x);//把x移除手牌
	void setPosition(int x, int y);//设置头像等个人信息的位置
	static void analyse_card(int* card, int num, int** list);//分析手牌
private:
	void Initial(RenderWindow* app);
	void show();//绘制
};

class Player :public User//玩家类
{
public:
	Player();
	void Initial(RenderWindow* app);
	void callDizhu(Event& e);//叫地主
	void show();
	Button bt_chupai, bt_pass, bt_callThree, bt_callTwo, bt_callOne, bt_callNo;
	Sprite sMeg;
	Texture tMeg;
};

class AI :public User//AI类
{
public:
	MyClock clock_callDizhu, clock_chupai;
	DFSCard dfs_card;
	AI();
	void Initial(RenderWindow* app);
	void callDizhu();
	void show();
};