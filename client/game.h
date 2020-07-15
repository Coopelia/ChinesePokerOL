#pragma once
#include"Def.h"
#include"scene.h"

class Game
{
public:
	RenderWindow* app;
	StartScene start_scene;//��ʼ����
	Scene* game_scene;//��Ϸ����
	bool OnStartScene;//�ڿ�ʼ����
	bool OnPlayScene;//����Ϸ����
	bool isOvered;//��Ϸ�ѽ����������˳�
	bool isOnWel;//��������
	int gameOver;//0δ���㣬1ʤ����2ʧ��
	Sprite sWel;
	Texture tWel;
	Game();
	~Game();
	void Initial_gameScene();
	void Update();//ÿִ֡��һ��
	void Input(Event& e);
	void Draw();
	void show_Wel(int i);//����
	void GameOver();//��Ϸ����
	void Run();//��Ϸ����
};