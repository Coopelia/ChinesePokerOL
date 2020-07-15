#include"MyClock.h"

MyClock::MyClock() //��ʼ���ֶ�
{
	hour = 0;
	minute = 0;
	second = 0;
	minTime = 0;
	this->endTime = 0;
	this->beginTime = 0;
	this->elapsedTime = 0;
	this->isRun = false;
}

void MyClock::stop()//ֹͣ�ͻص���ʼ״̬
{
	hour = 0;
	minute = 0;
	second = 0;
	minTime = 0;
	this->endTime = 0;
	this->beginTime = 0;
	this->elapsedTime = 0;
	this->isRun = false;
}

void MyClock::start()//��ʼ
{
	this->beginTime = clock();
	this->isRun = true;
}

void MyClock::restart()
{
	this->stop();
	this->start();
}

void MyClock::update()
{
	this->endTime = clock();
	if (this->isRun)
	{
		this->minTime += (this->endTime - this->beginTime);//����ʱ��-��ʼʱ��
		this->elapsedTime += (this->endTime - this->beginTime);
		this->second = elapsedTime / 1000;//�������
		if (this->second == 60)//����ɷ�
		{
			this->beginTime = this->endTime;
			this->elapsedTime = 0;
			this->minute++;
		}
		if (this->minute == 60)//�����ʱ
		{
			this->minute = 0;
			this->hour++;
		}
	}
	this->beginTime = this->endTime;//��һ֡��ʼʱ�������һ֡����ʱ��
}