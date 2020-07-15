#include"puke.h"

Card::Card()
{
	dt_e = 0;
	isSeleted = false;
	isOnTop = false;
	isPressed = false;
	isOnDesk = false;
	isDeleted = false;
	this->app = NULL;
	sbfx.loadFromFile("assets/Sound/Special/SpecSelectCard.ogg");
	fx.setBuffer(sbfx);
	fx.setLoop(false);
	sbf_shoot.loadFromFile("assets/Sound/2464.wav");
	sound_shoot.setBuffer(sbf_shoot);
	sound_shoot.setLoop(false);
}

void Card::onClick(Event& e)
{
	if (isOnDesk)
		return;
	FloatRect box = this->sprite.getGlobalBounds();
	int L, R, U, D;
	L = box.left;
	U = box.top;
	D = box.top + box.height;
	if (!isOnTop)
		R = box.left + dt_e;
	else
		R = box.left + box.width;
	if (mouseRect.isMousePressed)
	{
		if (mouseRect.isRectIn(px1,px2,py1,py2,L,R,U,D))
			this->isPressed = true;
		else
			this->isPressed = false;
	}
	if (this->isPressed)
	{
		this->sprite.setColor(Color(255, 255, 0, 255));
		this->fx.setVolume(vol_sound);
		this->fx.play();
	}
	else
		this->sprite.setColor(Color(255, 255, 255, 255));
}

bool Card::onShooted(RenderWindow* app, Event& e)
{
	bool flag = false;
	FloatRect box = sprite.getGlobalBounds();
	if (e.type == Event::MouseButtonReleased && e.key.code == Mouse::Left)
	{
		if (Mouse::getPosition(*app).x >= box.left && Mouse::getPosition(*app).x <= (box.left + box.width) && Mouse::getPosition(*app).y >= box.top && Mouse::getPosition(*app).y <= (box.top + box.height))
		{
			flag = true;
			this->sound_shoot.setVolume(vol_sound);
			this->sound_shoot.play();
		}
	}
	return flag;
}

Puke::Puke()
{
	size_x = 105;
	size_y = 150;

	t[0][0].loadFromFile("assets/image/puke/1/c03.png");
	t[0][1].loadFromFile("assets/image/puke/2/d03.png");
	t[0][2].loadFromFile("assets/image/puke/3/h03.png");
	t[0][3].loadFromFile("assets/image/puke/4/s03.png");

	t[1][0].loadFromFile("assets/image/puke/1/c04.png");
	t[1][1].loadFromFile("assets/image/puke/2/d04.png");
	t[1][2].loadFromFile("assets/image/puke/3/h04.png");
	t[1][3].loadFromFile("assets/image/puke/4/s04.png");

	t[2][0].loadFromFile("assets/image/puke/1/c05.png");
	t[2][1].loadFromFile("assets/image/puke/2/d05.png");
	t[2][2].loadFromFile("assets/image/puke/3/h05.png");
	t[2][3].loadFromFile("assets/image/puke/4/s05.png");

	t[3][0].loadFromFile("assets/image/puke/1/c06.png");
	t[3][1].loadFromFile("assets/image/puke/2/d06.png");
	t[3][2].loadFromFile("assets/image/puke/3/h06.png");
	t[3][3].loadFromFile("assets/image/puke/4/s06.png");

	t[4][0].loadFromFile("assets/image/puke/1/c07.png");
	t[4][1].loadFromFile("assets/image/puke/2/d07.png");
	t[4][2].loadFromFile("assets/image/puke/3/h07.png");
	t[4][3].loadFromFile("assets/image/puke/4/s07.png");

	t[5][0].loadFromFile("assets/image/puke/1/c08.png");
	t[5][1].loadFromFile("assets/image/puke/2/d08.png");
	t[5][2].loadFromFile("assets/image/puke/3/h08.png");
	t[5][3].loadFromFile("assets/image/puke/4/s08.png");

	t[6][0].loadFromFile("assets/image/puke/1/c09.png");
	t[6][1].loadFromFile("assets/image/puke/2/d09.png");
	t[6][2].loadFromFile("assets/image/puke/3/h09.png");
	t[6][3].loadFromFile("assets/image/puke/4/s09.png");
	
	t[7][0].loadFromFile("assets/image/puke/1/c10.png");
	t[7][1].loadFromFile("assets/image/puke/2/d10.png");
	t[7][2].loadFromFile("assets/image/puke/3/h10.png");
	t[7][3].loadFromFile("assets/image/puke/4/s10.png");

	t[8][0].loadFromFile("assets/image/puke/1/c11.png");
	t[8][1].loadFromFile("assets/image/puke/2/d11.png");
	t[8][2].loadFromFile("assets/image/puke/3/h11.png");
	t[8][3].loadFromFile("assets/image/puke/4/s11.png");

	t[9][0].loadFromFile("assets/image/puke/1/c12.png");
	t[9][1].loadFromFile("assets/image/puke/2/d12.png");
	t[9][2].loadFromFile("assets/image/puke/3/h12.png");
	t[9][3].loadFromFile("assets/image/puke/4/s12.png");

	t[10][0].loadFromFile("assets/image/puke/1/c13.png");
	t[10][1].loadFromFile("assets/image/puke/2/d13.png");
	t[10][2].loadFromFile("assets/image/puke/3/h13.png");
	t[10][3].loadFromFile("assets/image/puke/4/s13.png");
	
	t[11][0].loadFromFile("assets/image/puke/1/c01.png");
	t[11][1].loadFromFile("assets/image/puke/2/d01.png");
	t[11][2].loadFromFile("assets/image/puke/3/h01.png");
	t[11][3].loadFromFile("assets/image/puke/4/s01.png");

	t[12][0].loadFromFile("assets/image/puke/1/c02.png"); 
	t[12][1].loadFromFile("assets/image/puke/2/d02.png");
	t[12][2].loadFromFile("assets/image/puke/3/h02.png");
	t[12][3].loadFromFile("assets/image/puke/4/s02.png");

	t[13][0].loadFromFile("assets/image/puke/1/xw.png");
	t[13][1].loadFromFile("assets/image/puke/1/dw.png");

	tBack.loadFromFile("assets/image/puke/1/z01.png");
	Back.setTexture(tBack);
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			c[i][j].sprite.setTexture(t[i][j]);
			c[i][j].sprite.setPosition(-100, -100);
		}
	}
	c[13][0].sprite.setTexture(t[13][0]);
	c[13][1].sprite.setTexture(t[13][1]);
	c[13][0].sprite.setPosition(-100, -100);
	c[13][1].sprite.setPosition(-100, -100);
}