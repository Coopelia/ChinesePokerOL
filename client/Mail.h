#pragma once
#include"Def.h"
#include"Button.h"

class Mail
{
public:
	RenderWindow* app;
	Sprite sBack;//�ʼ������������
	Texture tBack;//�ʼ�������ͼ����
	Button bt_ok;//�ʼ�ȷ�ϰ�ť
	Text tittle, mes;//�ʼ����⣬�����ı�
	Font font;//����
	bool isOpened;//�Ƿ��Ѿ���
	int posX, posY;//λ��
	Mail();//���캯��
	void initial(RenderWindow* app, int x, int y);//��ʼ��
	void setText(String tittle, String mes);//�����ı�����
	void show();//����
};