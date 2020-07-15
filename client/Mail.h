#pragma once
#include"Def.h"
#include"Button.h"

class Mail
{
public:
	RenderWindow* app;
	Sprite sBack;//邮件背景精灵对象
	Texture tBack;//邮件背景贴图纹理
	Button bt_ok;//邮件确认按钮
	Text tittle, mes;//邮件标题，内容文本
	Font font;//字体
	bool isOpened;//是否已经打开
	int posX, posY;//位置
	Mail();//构造函数
	void initial(RenderWindow* app, int x, int y);//初始化
	void setText(String tittle, String mes);//设置文本内容
	void show();//绘制
};