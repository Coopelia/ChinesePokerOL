#pragma once
#include"Def.h"

class MyClock
{
public:
	int hour;//时
	int minute;//分
	int second;//秒
	float minTime;//总时间(ms)
	float elapsedTime;//两次调用update函数间隔时间
	bool isRun;//shidou
	clock_t beginTime, endTime;//用来记录开始时间和jieshu时间
	MyClock();
	void update();//每帧更新一次
	void start();//开始
	void stop();//停止
	void restart();//重新开始
};