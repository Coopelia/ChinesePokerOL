#include"Def.h"

int vol_bgm = 50;
int vol_sound = 50;
int jb = 0;
int px1 = 0;
int py1 = 0;
int px2 = 0;
int py2 = 0;
bool isRhythm = false;
bool isLoading = true;

void LoadAnim(RenderWindow* app)
{
	::std::cout << "���ڼ���......\n";
	Texture t;
	Sprite s;
	t.loadFromFile("assets/image/game/��¼/����/1.png");
	s.setTexture(t);
	s.setOrigin(75, 75);
	s.setPosition(640, 360);
	Clock cl;
	float et = 0;
	while (isLoading)
	{
		et += cl.restart().asMilliseconds();
		if (et >= 10)
		{
			et = 0;
			s.rotate(5);
		}
		app->clear();
		app->draw(s);
		app->display();
	}
	::std::cout << "�������\n";
	return;
}