#pragma once
#include"Def.h"

class Player//�����
{
public:
	SF sid;//���
	DEC dec;//����״̬
	::std::vector<int> hand_card;//������������
	int s_call;//�е����ķ�ֵ
	bool isCallingDizhu;//�Ƿ��ڽе���
	bool isMyTime;//�Ƿ��ֵ��Լ�����
	bool isWin;
	bool isReady;
	Player();
	void addCard(int x);//���x������
	void removeCard(int x);//��x�Ƴ�����
	void clear();
};