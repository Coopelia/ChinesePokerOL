#pragma once
#include"Def.h"

class Button
{
public:
	Button();
	bool PreBehRel;   //��ֹ����ڰ�ť���水סȻ���Ƶ���ť���ɿ���������¼�
	void setTextrue(String s);//��������
	void setPosition(int x, int y);//����λ��
	void setScale(float x, float y);//���ô�С
	void show();//����
	bool onClick(Event& e);//�����Ӧ����
	Sprite s;//�������
	Sound fx;//�����Ч
	SoundBuffer sbfx;//�����Чbuffer
	RenderWindow* app;//�󶨵Ļ����
private:
	Texture t;//������ͼ
};