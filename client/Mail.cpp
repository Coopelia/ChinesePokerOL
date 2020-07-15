#include "Mail.h"

Mail::Mail()//加载背景素材素材，这里统一一下邮件的风格
{
	font.loadFromFile("assets/fonts/fSimpleRound.ttf");
	tBack.loadFromFile("assets/image/game/大幅/大厅弹窗/邮件/mail_base.png");
	sBack.setTexture(tBack);
	bt_ok.setTextrue("assets/image/game/大厅弹框/#按钮/确定.png");
}

void Mail::initial(RenderWindow* app, int x, int y)//绑定活动窗口，设置位置
{
	this->app = app;
	bt_ok.app = app;
	isOpened = false;
	posX = x, posY = y;
	bt_ok.setScale(0.7, 0.7);//确认按钮大小
	bt_ok.setPosition(posX + 650, posY + 128);//确认按钮位置
	this->sBack.setPosition(posX, posY);
	this->tittle.setPosition(posX + 100, posY + 10);//标题
	this->mes.setPosition(posX + 12, posY + 60);//内容
}

void Mail::setText(String tittle, String mes)//设置标题为tittle，内容为mes，统一设置为一个下颜色
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

void Mail::show()//绘制到绑定的活动窗口
{
	(*app).draw(sBack);
	(*app).draw(tittle);
	(*app).draw(mes);
	bt_ok.show();
}