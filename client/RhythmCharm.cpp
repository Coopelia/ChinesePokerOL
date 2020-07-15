#include"RhythmCharm.h"

RhythmCharm::RhythmCharm()
{
	for (int i = 0; i < 54; i++) //��ʼ��
	{
		puke_pos[i].x = 0;
		puke_pos[i].y = 0;
		puke_now[i] = 1;
	}
	speed = 1;
	ytop = 0;
}

void RhythmCharm::start() //��ʼǰ��54���Ƶ�λ��������źã�ÿ4��һ�У�
{
	srand(time(0));
	speed = 1;
	ytop = -118;
	int temp[8], k; //temp��������ʱ���һ�е��Ƶ�λ�ã�0~3��Ӧ�������ҵ�0~3����4~7�͵�ǰ�в����ø���λ�ã�����ÿһ�в���������
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
				if (k < 4) //0~3��Ӧ�������ҵ�0~3��
				{
					puke_pos[i].x = k * 100 + 440;
					puke_pos[i].y = ytop;
					i++;
					if (i == 54)
						break;
				}
			}
		}
		ytop -= 147; // ÿ����һ�У�����������ô�൥λ��׼����һ�еĲ���
	}
	ytop += 147 * 4;
}

void RhythmCharm::update() //��speed���ٶ������ƶ����е���
{
	for (int i = 0; i < 54; i++)
	{
		puke_pos[i].y += 5 * speed;
		if (puke_pos[i].y > 722) //���˱߽�ͻص�������
			puke_pos[i].y = ytop;
	}
}