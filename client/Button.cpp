#include "Button.h"

Button::Button()
{
	this->app = NULL;
	this->PreBehRel = false;
	this->sbfx.loadFromFile("assets/Sound/Special/SpecOk.ogg");//���������ز�
	this->fx.setBuffer(sbfx);//��������
}

void Button::setTextrue(String s)//������ͼ
{
	this->t.loadFromFile(s);
	this->s.setTexture(this->t);
}

void Button::setPosition(int x, int y)//����λ��
{
	this->s.setPosition(x, y);
}

void Button::setScale(float x, float y)//���ô�С
{
	this->s.setScale(x, y);
}

bool Button::onClick(Event& e)//�������
{
	bool flag = false;
	FloatRect box = s.getGlobalBounds();//��ȡ��ť����
	if (Mouse::getPosition(*app).x >= box.left && Mouse::getPosition(*app).x <= (box.left + box.width) && Mouse::getPosition(*app).y >= box.top && Mouse::getPosition(*app).y <= (box.top + box.height))//����ڰ�ť�����ڵ��
	{
		if (e.type == Event::MouseButtonReleased && e.key.code == Mouse::Left && PreBehRel)//����ǰ��º��ɿ����ڰ�ť��һ���ǳɾ���true
		{
			this->PreBehRel = false;
			flag = true;
			this->fx.setVolume(vol_sound);
			this->fx.play();
		}
		else
			flag = false;
		if (e.type == Event::MouseButtonPressed && e.key.code == Mouse::Left) //������°�ť����ɫ��ƻ�ɫ
		{
			this->s.setColor(Color(125, 125, 0, 255));
			this->PreBehRel = true;
		}
		else
			this->s.setColor(Color(125, 125, 125, 255));//������ֻ��ͣ�ڰ�ť���沢δ���£���ɫ
	}
	else
		this->s.setColor(Color(255,255,255, 255));//ԭ������ɫ
	return flag;
}

void Button::show()
{
	(*app).draw(s);
}