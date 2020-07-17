#pragma once
#include"Def.h"
#include"Button.h"
#include"MyClock.h"
#include"PukeManager.h"
#include"OLPukeManager.h"
#include"Mail.h"
#include"RoomGUI.h"

enum SceneType
{
	danji,
	oline
};

class Scene
{
public:
	Music bgm;
	Sprite sBackground; //����
	Font font;//����
	::std::vector<Texture> tBack;
	::std::unordered_map<::std::string, bool> bool_list;//bool list
	RenderWindow* app;//�����
	int value_bg;//����ͼƬ���

	//���ò˵�
	Sprite sBack_setting, sBackMini;
	Texture tBack_setting;
	Button bt_setting_ok, bt_bgm_left, bt_bgm_right, bt_sound_left, bt_sound_right, bt_bg_left, bt_bg_right, bt_rhythm_left, bt_rhythm_right;
	Text text_bgm, text_sound, text_rhythm;
	void input_setting(Event& e);
	void draw_setting(RenderWindow* app);
	//�˳��Ի���
	Sprite sBack_exit;
	Texture tBack_exit;
	Button bt_exit_ok, bt_exit_cancel;
	void input_exit(Event& e);
	void draw_exit(RenderWindow* app);
	//���ӳ���Ի���
	Sprite sBack_error;
	Texture tBack_error;
	Button bt_error_ok;
	void input_error(RenderWindow* app, Event& e);
	void draw_error(RenderWindow* app);

	Scene();
	virtual ~Scene() {};
	virtual void Initial_window(RenderWindow* app) {};
	virtual void Initial_assets(){};//�����ز�
	virtual void Start() {};//���볡��ʱִ��һ��
	virtual void Update() {};//���볡����ûִ֡��һ��
	virtual void Input(Event& e) {};//����������
	virtual void Draw() {};//����������
	virtual void SceneClose() {};//�رճ���
};

class StartScene: public Scene //��ʼ������
{
	friend class Game;
public:
	StartScene();
	void Initial_window(RenderWindow* app);
	void Initial_assets();//�����ز�
	void Start();//���볡��ʱִ��һ��
	void Update();//���볡����ûִ֡��һ��
	void Input(Event& e);//����������
	void Draw();//����������
	void SceneClose();//�رճ���
	SceneType next_scene;//��һ����������
	Button bt_Enter, bt_creat, bt_fankui, bt_store, bt_wanfa, bt_mail, bt_zhanji, bt_exit, bt_setting;
	Sprite sMeinv, sBackUser, sUserHead, sJf, sRankList, sBack_menu;
	Texture tMeinv, tBackUser, tUserHead, tJf, tRankList, tBack_menu;
	Text text_jb;
	void input_main(Event& e);
	void draw_main();
	//�ʼ��˵�
	int num_mail;
	Sprite sBack_mail, sRedPoint;
	Texture tBack_mail, tRedPoint;
	Button bt_mail_close;
	Mail mail;
	void input_mail(Event& e);
	void draw_mail();
	//�淨�˵�
	Sprite sBack_wanfa;
	Texture tBack_wanfa;
	Button bt_wanfa_ok;
	void input_wanfa(Event& e);
	void draw_wanfa();
	//�����˵�
	Sprite sBack_fankui;
	Texture tBack_fankui;
	Button bt_fankui_close;
	void input_fankui(Event& e);
	void draw_fankui();
	//ս���˵�
	Sprite sBack_zhanji;
	Texture tBack_zhanji;
	Button bt_zhanji_close;
	void input_zhanji(Event& e);
	void draw_zhanji();
	//�̵�˵�
	Sprite sBack_store;
	Texture tBack_store;
	Button bt_store_close;
	void input_store(Event& e);
	void draw_store();
	//������Ϸ
	RoomGUI room_menu;
	Button bt_join, bt_join_exit;
	void input_join(Event& e);
	void draw_join();
};

class GameScene: public Scene //��������Ϸ�����ࣨ������
{
	friend class Game;
public:
	PukeManager puke_manager;//�˿˹�����
	Player human;//���
	AI ai_1, ai_2;//AI
	Sprite sOver, sShoot, sDealBg, sF, sG, sH, sJ;
	Texture tOver[2], tShoot, tDealBg, tF, tG, tH, tJ;
	Text text_score, text_jb, text_over, text_shoot;
	Button bt_over_back, bt_over_restart, bt_exit, bt_setting;
	Sound sound_get;
	SoundBuffer sb_get;
	int score;
	int elapsTime_shoot;
	int totalTime_shoot;
	Music mu_over;
	MyClock clock_showCall;//���Ƶ���ʱ
	Clock clock_shoot;
	GameScene();
	void Initial_window(RenderWindow* app);
	void Initial_assets();//�����ز�
	void Start();//���볡��ʱִ��һ��
	void Update();//���볡����ûִ֡��һ��
	void Input(Event& e);//����������
	void Draw();//����������
	void SceneClose();//�رճ���
	void player_turn_input(Event& e);//����������ƺͽе���
	void input_rhythm(Event& e); //����ģʽ����
private:
	RenderWindow* app;
};

class GameSceneOL :public Scene//����
{
	friend class Game;
public:
	OLPukeManager puke_manager;//�˿˹�����
	Player human_self,human_1,human_2;//���
	Sprite sOver, sShoot, sDealBg, sF, sG, sH, sJ;
	Texture tOver[2], tShoot, tDealBg, tF, tG, tH, tJ;
	Text text_score, text_jb, text_over, text_shoot;
	Button bt_over_back, bt_over_restart, bt_exit, bt_setting;
	Sound sound_get;
	SoundBuffer sb_get;
	int score;
	Music mu_over;
	MyClock clock_showCall;//���Ƶ���ʱ
	GameSceneOL();
	void Initial_window(RenderWindow* app);
	void Initial_assets();//�����ز�
	void Start();//���볡��ʱִ��һ��
	void Update();//���볡����ûִ֡��һ��
	void Input(Event& e);//����������
	void Draw();//����������
	void SceneClose();//�رճ���
	void player_turn_input(Event& e);//����������ƺͽе���
private:
	RenderWindow* app;
	int player_turned_id;
};