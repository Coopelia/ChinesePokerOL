#pragma once
#include"Def.h"

class RhythmCharm
{
public:
	Vector2i puke_pos[54]; //54���Ƶ�����
	int puke_now[54]; //δ��ȡ�ߵ���Ϊ1����ȡ�ߵ���Ϊ0
	int ytop; //��������Ƶ�y����
	float speed; //���µ����ٶ�
	RhythmCharm();
	void start(); //��ʼǰִ��һ��
	void update();//ÿִ֡��һ��
};