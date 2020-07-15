#pragma once
#include"Def.h"

typedef struct Pos
{
	int x;
	int y;
	Pos() :x(0), y(0) {};
}Pos;

typedef struct Scale
{
	float x;
	float y;
	Scale() :x(0.5), y(0.5) {};
}Scale;

typedef struct Speed
{
	float x;
	float y;
	Speed() :x(0), y(0) {};
}Speed;

class GunCharm
{
public:
	GunCharm();//构造函数
	void start();//开始执行一次
	void updata();//开始后每帧执行一次
	void clear();//清空
	Pos puke_pos[20];//扑克牌坐标
	Speed speed_mov[20];//扑克牌速度
	Speed acc[20];//扑克牌加速度
	float speed_sca[20];//扑克牌缩放速度
	float puke_scale[20];//扑克牌缩放比例
	int num;//扑克牌数量
	Clock clock_mov, clock_spe;//计时器
	float elapsTime_mov, elapsTime_spe;//时间间隔
};