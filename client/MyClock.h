#pragma once
#include"Def.h"

class MyClock
{
public:
	int hour;//ʱ
	int minute;//��
	int second;//��
	float minTime;//��ʱ��(ms)
	float elapsedTime;//���ε���update�������ʱ��
	bool isRun;//shidou
	clock_t beginTime, endTime;//������¼��ʼʱ���jieshuʱ��
	MyClock();
	void update();//ÿ֡����һ��
	void start();//��ʼ
	void stop();//ֹͣ
	void restart();//���¿�ʼ
};