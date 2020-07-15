#include "Mail.h"

Mail::Mail()//���ر����ز��زģ�����ͳһһ���ʼ��ķ��
{
	font.loadFromFile("assets/fonts/fSimpleRound.ttf");
	tBack.loadFromFile("assets/image/game/���/��������/�ʼ�/mail_base.png");
	sBack.setTexture(tBack);
	bt_ok.setTextrue("assets/image/game/��������/#��ť/ȷ��.png");
}

void Mail::initial(RenderWindow* app, int x, int y)//�󶨻���ڣ�����λ��
{
	this->app = app;
	bt_ok.app = app;
	isOpened = false;
	posX = x, posY = y;
	bt_ok.setScale(0.7, 0.7);//ȷ�ϰ�ť��С
	bt_ok.setPosition(posX + 650, posY + 128);//ȷ�ϰ�ťλ��
	this->sBack.setPosition(posX, posY);
	this->tittle.setPosition(posX + 100, posY + 10);//����
	this->mes.setPosition(posX + 12, posY + 60);//����
}

void Mail::setText(String tittle, String mes)//���ñ���Ϊtittle������Ϊmes��ͳһ����Ϊһ������ɫ
{
	this->tittle.setString(tittle);
	this->tittle.setCharacterSize(36);
	this->tittle.setFillColor(Color::Blue);
	this->tittle.setFont(font);
	this->mes.setString(mes);
	this->mes.setCharacterSize(42);
	this->mes.setFillColor(Color::Black);
	this->mes.setFont(font);
}

void Mail::show()//���Ƶ��󶨵Ļ����
{
	(*app).draw(sBack);
	(*app).draw(tittle);
	(*app).draw(mes);
	bt_ok.show();
}