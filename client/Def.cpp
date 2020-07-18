#include"Def.h"

int vol_bgm = 50;
int vol_sound = 50;
int jb = 0;
int px1 = 0;
int py1 = 0;
int px2 = 0;
int py2 = 0;
int roomId = -1;
bool isRhythm = false;
bool isLoading = true;
bool isExit = false;

void LoadAnim()
{
	RenderWindow* app;
	::sf::WindowHandle h;
	Texture t;
	Sprite s;
	t.loadFromFile("assets/image/game/登录/加载/1.png");
	s.setTexture(t);
	s.setOrigin(75, 75);
	s.setPosition(180, 180);
	Clock cl;
	float et = 0;
	while (!isExit)
	{
		if (isLoading)
		{
			::std::cout << "正在加载......\n";
			app = new RenderWindow(VideoMode(360, 360), "Loading");
			app->setFramerateLimit(60);
			h = app->getSystemHandle();
			::SetWindowPos(h, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
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
				app->requestFocus();
				app->display();
			}
			::std::cout << "加载完成\n";
			app->close();
			delete app;
		}
	}

	return;
}