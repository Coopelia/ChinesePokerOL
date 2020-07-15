#include"Def.h"
#include"MouseRectIn.h"
#pragma once

class Card //���ࣨһ�ţ�
{
public:
	Card();
	RenderWindow* app;
	Sprite sprite;
	Sound fx;//�����Ч
	SoundBuffer sbfx;
	Sound sound_shoot;//��������Ч
	SoundBuffer sbf_shoot;
	int dt_e;
	bool isSeleted; //�Ƿ�ѡ��
	bool isOnTop;//������
	bool isPressed;//�Ƿ񱻰���
	bool isOnDesk;//�Ƿ��ڳ�����
	bool isDeleted;//�Ƿ��ѱ�����
	void onClick(Event& e);//�����Ӧ����
	bool onShooted(RenderWindow* app, Event& e);//�����Ӧ����
};

class Puke//�˿��ࣨһ���ƣ�
{
public:
	bool isMousePressed;  //��ť�Ƿ���
	int size_x;//�˿��ƿ�
	int size_y;//�˿��Ƹ�
	float scale;//���ű���
	Sprite Back; //����
	Card c[14][4]; //14���ƣ�ÿ��4�ֻ�ɫ
	Puke();
private:
	Texture t[14][4];//����������ͼ
	Texture tBack;//����
};