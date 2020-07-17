#pragma once
#include"Def.h"
#include"scene.h"
#include"Connection.h"

class Game
{
public:
	Game();
	~Game();
	void Update();//ÿִ֡��һ��
	void Input(Event& e);
	void Draw();
	void show_Wel(int i);//����
	void GameOver();//��Ϸ����
	void InitialGameScene();//��ʼ����һ������
	static void sender();//�������߳�
	static void reciever();//�������߳�
	void Run();//��Ϸ����
private:
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
};