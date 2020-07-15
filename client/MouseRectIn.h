#pragma once
#include"Def.h"

class MouseRectIn
{
public:
	bool isMousePressed;//鼠标是否处于按下状态
	MouseRectIn();//构造函数
	bool isRectIn(int x01, int x02, int y01, int y02, int x11, int x12, int y11, int y12);//矩形是否在鼠标拉选区域范围内
};
extern MouseRectIn mouseRect;