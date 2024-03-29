#pragma once
#include"Def.h"
#include"Player.h"
#include"puke.h"
#include"GunCharm.h"
#include"RhythmCharm.h"
#include"MsgQueue.h"
#include"Connection.h"

class OLPukeManager
{
	friend class GameSceneOL;
public:
	OLPukeManager();
	void start();//start before the first frame
	void update();//每帧执行一次
	void clearAll();//清空所有牌的状态
	void deal();//发牌
	void addToJudge(int i);//将选中的牌添加到判断队列
	bool handSeletedCard();//打出选中的牌
private:
	int puke_dt_x, puke_dt_e;//玩家手牌区牌的位置参数
	int puke_chupai_lx, puke_chupai_dx;//出牌区牌的位置参数
	int num_desk, num_seleted;
	int dizhuCard[3];//地主牌
	int deskCard[21];//出牌区的牌
	int seletedCard[21];//选中要出的牌
	Card sDizhuCard[3];
	Player* human_self, * human_1, * human_2;
	Puke puke;
	MyClock clock_deal;//发牌计时器，用于实现0.3S发一张牌的动画
	PukeType checkType(int* card, int num);
	void sort_seleted();//对判断队列排序
	int player_turned_id;
	bool IsFeafible();
};