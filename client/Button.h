#pragma once
#include"Def.h"

class Button
{
public:
	Button();
	bool PreBehRel;   //防止鼠标在按钮外面按住然后移到按钮上松开触发点击事件
	void setTextrue(String s);//设置纹理
	void setPosition(int x, int y);//设置位置
	void setScale(float x, float y);//设置大小
	void show();//绘制
	bool onClick(Event& e);//点击响应函数
	Sprite s;//精灵对象
	Sound fx;//点击音效
	SoundBuffer sbfx;//点击音效buffer
	RenderWindow* app;//绑定的活动窗口
private:
	Texture t;//纹理贴图
};