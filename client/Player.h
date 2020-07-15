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

class User//�û���
{
public:
	RenderWindow* app;
	SF sid;//���
	DEC dec;//����״̬
	int hand_card[20];//������������
	int num_card;//��������
	int s_call;//�е����ķ�ֵ
	bool isCallingDizhu;//�Ƿ��ڽе���
	bool isMyTime;//�Ƿ��ֵ��Լ�����
	bool isWin;
	MyClock clock_daojishi;//���Ƶ���ʱ
	Sprite sHead, sCall, sNoCard, sClock;
	Texture tHead[3], tCallOne, tCallTwo, tCallThree, tCallNo, tNoCard, tClock;
	Font font;
	Text tNum_rest, tDaojishi;
	User();
	void update();//ÿ֡����
	void sort();//��������
	void addCard(int x);//���x������
	void removeCard(int x);//��x�Ƴ�����
	void setPosition(int x, int y);//����ͷ��ȸ�����Ϣ��λ��
	static void analyse_card(int* card, int num, int** list);//��������
private:
	void Initial(RenderWindow* app);
	void show();//����
};

class Player :public User//�����
{
public:
	Player();
	void Initial(RenderWindow* app);
	void callDizhu(Event& e);//�е���
	void show();
	Button bt_chupai, bt_pass, bt_callThree, bt_callTwo, bt_callOne, bt_callNo;
	Sprite sMeg;
	Texture tMeg;
};

class AI :public User//AI��
{
public:
	MyClock clock_callDizhu, clock_chupai;
	DFSCard dfs_card;
	AI();
	void Initial(RenderWindow* app);
	void callDizhu();
	void show();
};