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
	void update();//ÿִ֡��һ��
	void clearAll();//��������Ƶ�״̬
	void deal();//����
	void deal_dizhuCard();//��������
	void addToJudge(int i);//��ѡ�е�����ӵ��ж϶���
	bool handSeletedCard();//���ѡ�е���
private:
	int puke_dt_x, puke_dt_e;//����������Ƶ�λ�ò���
	int puke_chupai_lx, puke_chupai_dx;//�������Ƶ�λ�ò���
	::std::vector<int> dizhuCard;//������
	::std::vector<int> deskCard;//����������
	::std::vector<int> seletedCard;//ѡ��Ҫ������
	Card sDizhuCard[3];
	Player* human_self, * human_1, * human_2;
	Puke puke;
	MyClock clock_deal;//���Ƽ�ʱ��������ʵ��0.3S��һ���ƵĶ���
	PukeType checkType(::std::vector<int>& cards);
	int player_turned_id;
	bool IsFeafible();
};