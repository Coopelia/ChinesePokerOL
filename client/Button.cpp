#include "Button.h"

Button::Button()
{
	this->app = NULL;
	this->PreBehRel = false;
	this->sbfx.loadFromFile("assets/Sound/Special/SpecOk.ogg");//加载声音素材
	this->fx.setBuffer(sbfx);//设置声音
}

void Button::setTextrue(String s)//设置贴图
{
	this->t.loadFromFile(s);
	this->s.setTexture(this->t);
}

void Button::setPosition(int x, int y)//设置位置
{
	this->s.setPosition(x, y);
}

void Button::setScale(float x, float y)//设置大小
{
	this->s.setScale(x, y);
}

bool Button::onClick(Event& e)//点击函数
{
	bool flag = false;
	FloatRect box = s.getGlobalBounds();//获取按钮区域
	if (Mouse::getPosition(*app).x >= box.left && Mouse::getPosition(*app).x <= (box.left + box.width) && Mouse::getPosition(*app).y >= box.top && Mouse::getPosition(*app).y <= (box.top + box.height))//如果在按钮区域内点击
	{
		if (e.type == Event::MouseButtonReleased && e.key.code == Mouse::Left && PreBehRel)//如果是按下和松开是在按钮上一气呵成就是true
		{
			this->PreBehRel = false;
			flag = true;
			this->fx.setVolume(vol_sound);
			this->fx.play();
		}
		else
			flag = false;
		if (e.type == Event::MouseButtonPressed && e.key.code == Mouse::Left) //如果按下按钮，颜色变黄灰色
		{
			this->s.setColor(Color(125, 125, 0, 255));
			this->PreBehRel = true;
		}
		else
			this->s.setColor(Color(125, 125, 125, 255));//如果鼠标只是停在按钮上面并未按下，灰色
	}
	else
		this->s.setColor(Color(255,255,255, 255));//原来的颜色
	return flag;
}

void Button::show()
{
	(*app).draw(s);
}