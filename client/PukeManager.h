#pragma once
#include"Def.h"
#include"Player.h"
#include"puke.h"
#include"GunCharm.h"
#include"RhythmCharm.h"

class PukeManager
{
public:
	int dizhuCard[3];//地主牌
	int puke_dt_x, puke_dt_e;//玩家手牌区牌的位置参数
	int puke_chupai_lx, puke_chupai_dx;//出牌区牌的位置参数
	int num_desk, num_seleted, num_temp;//出牌区、选中的牌、辅助队列牌数
	int deskCard[20];//出牌区的牌
	int seletedCard[20];//选中要出的牌
	Card sDizhuCard[3];
	Player* human;
	AI* ai_1;//右边的ai
	AI* ai_2;//左边的ai
	Puke puke;
	MyClock clock_deal;//发牌计时器，用于实现0.3S发一张牌的动画
	DFSCard dfs_card;//计算手牌次数
	GunCharm gunCharm;//射击
	RhythmCharm rhythmCharm;//节奏
	bool isGunCharm;
	PukeManager();
	void Start();//start before the first frame
	void update();//每帧执行一次
	void clearAll();//清空所有牌的状态
	void deal();//发牌
	void getCard_rhythm(int n);
	void deal_dizhuCard();//发地主牌
	void addToJudge(int i, int j);//将选中的牌添加到判断队列
	void removeFromDesk(int i);//从桌面移除牌
	void sort_seleted();//对判断队列排序
	void autoSeleteCard(AI* ai, PukeType& type);//AI自动选牌
	bool findWithType(PukeType type, int low, int* source_card, int* result_card);//从source_card中找最合适的type类型的卡,主体部分最小为low
	PukeType checkType(int* card, int num);//判断牌型（三带等）
	void JudgeCard(Player& human, PukeType& type);//判断是否合法
private://特殊模式下辅助数组
	int temp[54];
	int dealTemp[54];
	int puke_order[54];
};