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
	void addToJudge(int i);//��ѡ�е�����ӵ��ж϶���
	bool handSeletedCard();//���ѡ�е���
private:
	int puke_dt_x, puke_dt_e;//����������Ƶ�λ�ò���
	int puke_chupai_lx, puke_chupai_dx;//�������Ƶ�λ�ò���
	int num_desk, num_seleted;
	int dizhuCard[3];//������
	int deskCard[21];//����������
	int seletedCard[21];//ѡ��Ҫ������
	Card sDizhuCard[3];
	Player* human_self, * human_1, * human_2;
	Puke puke;
	MyClock clock_deal;//���Ƽ�ʱ��������ʵ��0.3S��һ���ƵĶ���
	PukeType checkType(int* card, int num);
	void sort_seleted();//���ж϶�������
	int player_turned_id;
	bool IsFeafible();
};