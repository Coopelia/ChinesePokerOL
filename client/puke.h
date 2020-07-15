#include"Def.h"
#include"MouseRectIn.h"
#pragma once

class Card //牌类（一张）
{
public:
	Card();
	RenderWindow* app;
	Sprite sprite;
	Sound fx;//点击音效
	SoundBuffer sbfx;
	Sound sound_shoot;//被射中音效
	SoundBuffer sbf_shoot;
	int dt_e;
	bool isSeleted; //是否被选中
	bool isOnTop;//最大的牌
	bool isPressed;//是否被按下
	bool isOnDesk;//是否在出牌区
	bool isDeleted;//是否已被出掉
	void onClick(Event& e);//点击响应函数
	bool onShooted(RenderWindow* app, Event& e);//射击响应函数
};

class Puke//扑克类（一副牌）
{
public:
	bool isMousePressed;  //按钮是否按下
	int size_x;//扑克牌宽
	int size_y;//扑克牌高
	float scale;//缩放比例
	Sprite Back; //卡背
	Card c[14][4]; //14种牌，每种4种花色
	Puke();
private:
	Texture t[14][4];//卡面纹理贴图
	Texture tBack;//卡背
};