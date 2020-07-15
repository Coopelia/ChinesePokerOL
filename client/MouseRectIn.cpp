#include "MouseRectIn.h"

MouseRectIn::MouseRectIn()
{
	isMousePressed = false;
}

bool MouseRectIn::isRectIn(int x01, int x02, int y01, int y02, int x11, int x12, int y11, int y12)//连个矩形是否相交算法
{
	int zx = abs(x01 + x02 - x11 - x12);
	int x = abs(x01 - x02) + abs(x11 - x12);
	int zy = abs(y01 + y02 - y11 - y12);
	int y = abs(y01 - y02) + abs(y11 - y12);
	if (zx <= x && zy <= y)
		return true;
	else
		return false;
}
MouseRectIn mouseRect;
