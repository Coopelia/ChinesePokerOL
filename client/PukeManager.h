#pragma once
#include"Def.h"
#include"Player.h"
#include"puke.h"
#include"GunCharm.h"
#include"RhythmCharm.h"

class PukeManager
{
public:
	int dizhuCard[3];//������
	int puke_dt_x, puke_dt_e;//����������Ƶ�λ�ò���
	int puke_chupai_lx, puke_chupai_dx;//�������Ƶ�λ�ò���
	int num_desk, num_seleted, num_temp;//��������ѡ�е��ơ�������������
	int deskCard[20];//����������
	int seletedCard[20];//ѡ��Ҫ������
	Card sDizhuCard[3];
	Player* human;
	AI* ai_1;//�ұߵ�ai
	AI* ai_2;//��ߵ�ai
	Puke puke;
	MyClock clock_deal;//���Ƽ�ʱ��������ʵ��0.3S��һ���ƵĶ���
	DFSCard dfs_card;//�������ƴ���
	GunCharm gunCharm;//���
	RhythmCharm rhythmCharm;//����
	bool isGunCharm;
	PukeManager();
	void Start();//start before the first frame
	void update();//ÿִ֡��һ��
	void clearAll();//��������Ƶ�״̬
	void deal();//����
	void getCard_rhythm(int n);
	void deal_dizhuCard();//��������
	void addToJudge(int i, int j);//��ѡ�е�����ӵ��ж϶���
	void removeFromDesk(int i);//�������Ƴ���
	void sort_seleted();//���ж϶�������
	void autoSeleteCard(AI* ai, PukeType& type);//AI�Զ�ѡ��
	bool findWithType(PukeType type, int low, int* source_card, int* result_card);//��source_card��������ʵ�type���͵Ŀ�,���岿����СΪlow
	PukeType checkType(int* card, int num);//�ж����ͣ������ȣ�
	void JudgeCard(Player& human, PukeType& type);//�ж��Ƿ�Ϸ�
private://����ģʽ�¸�������
	int temp[54];
	int dealTemp[54];
	int puke_order[54];
};