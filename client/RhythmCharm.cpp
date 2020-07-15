#include"RhythmCharm.h"

RhythmCharm::RhythmCharm()
{
	for (int i = 0; i < 54; i++) //初始化
	{
		puke_pos[i].x = 0;
		puke_pos[i].y = 0;
		puke_now[i] = 1;
	}
	speed = 1;
	ytop = 0;
}

void RhythmCharm::start() //开始前将54张牌的位置随机安排好（每4张一行）
{
	srand(time(0));
	speed = 1;
	ytop = -118;
	int temp[8], k; //temp是用来临时存放一行的牌的位置，0~3对应从左往右第0~3个，4~7就当前行不设置该牌位置，所以每一行并不是满的
	for (int i = 0; i < 54; i)
	{
		if (puke_now[i] == 0)
		{	
			i++;
			continue;
		}
		for (int ii = 0; ii < 8; ii++)
			temp[ii] = 1;
		for (int j = 0; j < 4; j)
		{
			k = rand() % 8;
			if (temp[k] == 1)
			{
				j++;
				temp[k] = 0;
				if (k < 4) //0~3对应从左往右第0~3个
				{
					puke_pos[i].x = k * 100 + 440;
					puke_pos[i].y = ytop;
					i++;
					if (i == 54)
						break;
				}
			}
		}
		ytop -= 147; // 每布置一行，就往上移这么多单位，准备下一行的布置
	}
	ytop += 147 * 4;
}

void RhythmCharm::update() //以speed的速度向下移动所有的牌
{
	for (int i = 0; i < 54; i++)
	{
		puke_pos[i].y += 5 * speed;
		if (puke_pos[i].y > 722) //到了边界就回到最上面
			puke_pos[i].y = ytop;
	}
}