#pragma once
#include"Def.h"
#include"scene.h"
#include"Connection.h"

class Game
{
public:
	Game();
	~Game();
	void Update();//每帧执行一次
	void Input(Event& e);
	void Draw();
	void show_Wel(int i);//开场
	void GameOver();//游戏结束
	void InitialGameScene();//初始化下一个场景
	static void sender();//发送者线程
	static void reciever();//接收者线程
	void Run();//游戏运行
private:
	RenderWindow* app;
	StartScene start_scene;//开始场景
	Scene* game_scene;//游戏场景
	bool OnStartScene;//在开始场景
	bool OnPlayScene;//在游戏场景
	bool isOvered;//游戏已结束，即将退出
	bool isOnWel;//开场画面
	int gameOver;//0未结算，1胜利，2失败
	Sprite sWel;
	Texture tWel;
};