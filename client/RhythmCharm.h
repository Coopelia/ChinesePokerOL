#pragma once
#include"Def.h"

class RhythmCharm
{
public:
	Vector2i puke_pos[54]; //54张牌的坐标
	int puke_now[54]; //未被取走的牌为1，被取走的牌为0
	int ytop; //最上面的牌的y坐标
	float speed; //向下掉的速度
	RhythmCharm();
	void start(); //开始前执行一次
	void update();//每帧执行一次
};