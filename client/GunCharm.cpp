#include"GunCharm.h"

GunCharm::GunCharm()
{
	num = 0;
	elapsTime_mov = 0;
	elapsTime_spe = 0;
	memset(speed_sca, 0, sizeof(speed_sca));
	memset(puke_scale, 0.5, sizeof(puke_scale));
}

void GunCharm::start()
{
	clock_mov.restart();
	clock_spe.restart();
	srand(time(0));
	for (int i = 0; i < num; i++)
	{
		speed_mov[i].x = rand() % 10 - 5;
		speed_mov[i].y = rand() % 10 - 5;
		speed_sca[i] = rand() % 10 - 5;
		acc[i].x = rand() % 6 - 3;
		acc[i].y = rand() % 6 - 3;
	}
}

void GunCharm::updata()
{
	elapsTime_mov += clock_mov.restart().asMilliseconds();
	elapsTime_spe += clock_spe.restart().asMilliseconds();
	if (elapsTime_spe > 1000)
	{
		elapsTime_spe = 0;
		srand(time(0));
		for (int i = 0; i < num; i++)
		{
			speed_mov[i].x = rand() % 10 - 5;
			speed_mov[i].y = rand() % 10 - 5;
			speed_sca[i] = rand() % 10 - 5;
			acc[i].x = rand() % 6 - 3;
			acc[i].y = rand() % 6 - 3;
		}
	}
	if (elapsTime_mov > 100)
	{
		elapsTime_mov = 0;
		for (int i = 0; i < num; i++)
		{
			if (puke_pos[i].x <= 100 || puke_pos[i].x >= WIDTH - 200)
			{
				speed_mov[i].x = -speed_mov[i].x;
				acc[i].x = -acc[i].x;
			}
			if (puke_pos[i].y <= 20 || puke_pos[i].y >= HEIGHT - 200)
			{
				speed_mov[i].y = -speed_mov[i].y;
				acc[i].y = -acc[i].y;
			}
			if (puke_scale[i] <= 0.2 || puke_scale[i] >= 1)
			{
				speed_sca[i] = -speed_sca[i];
			}
			speed_mov[i].x += acc[i].x;
			speed_mov[i].y += acc[i].y;
			puke_pos[i].x += speed_mov[i].x;
			puke_pos[i].y += speed_mov[i].y;
			puke_scale[i] += speed_sca[i] / 100;
		}
	}
}

void GunCharm::clear()
{
	memset(puke_pos, 0, sizeof(puke_pos));
	memset(puke_scale, 0.5, sizeof(puke_scale));
	memset(speed_mov, 0, sizeof(speed_mov));
	memset(speed_sca, 0, sizeof(speed_sca));
	memset(speed_sca, 0, sizeof(speed_sca));
	memset(acc, 0, sizeof(acc));
	num = 0;
	elapsTime_mov = 0;
	elapsTime_spe = 0;
}