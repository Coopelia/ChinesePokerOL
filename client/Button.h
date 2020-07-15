#pragma once
#include"Def.h"

class Button
{
	friend class Game;
	friend class Scene;
	friend class StartScene;
	friend class GameScene;
	friend class GameSceneOL;
	friend class Menu;
	friend class Mail;
	friend class Player;
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
private:
	Texture t;//������ͼ
	RenderWindow* app;//�󶨵Ļ����
};