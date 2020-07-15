#include"MyClock.h"

MyClock::MyClock() //初始化字段
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

void MyClock::stop()//停止就回到初始状态
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

void MyClock::start()//开始
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
		this->minTime += (this->endTime - this->beginTime);//结束时间-开始时间
		this->elapsedTime += (this->endTime - this->beginTime);
		this->second = elapsedTime / 1000;//换算成秒
		if (this->second == 60)//换算成分
		{
			this->beginTime = this->endTime;
			this->elapsedTime = 0;
			this->minute++;
		}
		if (this->minute == 60)//换算成时
		{
			this->minute = 0;
			this->hour++;
		}
	}
	this->beginTime = this->endTime;//下一帧开始时间就是这一帧结束时间
}