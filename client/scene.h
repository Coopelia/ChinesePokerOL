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
	Sprite sBackground; //背景
	Font font;//字体
	::std::vector<Texture> tBack;
	::std::unordered_map<::std::string, bool> bool_list;//bool list
	RenderWindow* app;//活动窗口
	int value_bg;//背景图片编号

	//设置菜单
	Sprite sBack_setting, sBackMini;
	Texture tBack_setting;
	Button bt_setting_ok, bt_bgm_left, bt_bgm_right, bt_sound_left, bt_sound_right, bt_bg_left, bt_bg_right, bt_rhythm_left, bt_rhythm_right;
	Text text_bgm, text_sound, text_rhythm;
	void input_setting(Event& e);
	void draw_setting(RenderWindow* app);
	//退出对话框
	Sprite sBack_exit;
	Texture tBack_exit;
	Button bt_exit_ok, bt_exit_cancel;
	void input_exit(Event& e);
	void draw_exit(RenderWindow* app);
	//连接出错对话框
	Sprite sBack_error;
	Texture tBack_error;
	Button bt_error_ok;
	void input_error(RenderWindow* app, Event& e);
	void draw_error(RenderWindow* app);

	Scene();
	virtual ~Scene() {};
	virtual void Initial_window(RenderWindow* app) {};
	virtual void Initial_assets(){};//加载素材
	virtual void Start() {};//载入场景时执行一次
	virtual void Update() {};//载入场景后没帧执行一次
	virtual void Input(Event& e) {};//输入主函数
	virtual void Draw() {};//绘制主函数
	virtual void SceneClose() {};//关闭场景
};

class StartScene: public Scene //开始场景类
{
	friend class Game;
public:
	StartScene();
	void Initial_window(RenderWindow* app);
	void Initial_assets();//加载素材
	void Start();//载入场景时执行一次
	void Update();//载入场景后没帧执行一次
	void Input(Event& e);//输入主函数
	void Draw();//绘制主函数
	void SceneClose();//关闭场景
	SceneType next_scene;//下一个场景类型
	Button bt_Enter, bt_creat, bt_fankui, bt_store, bt_wanfa, bt_mail, bt_zhanji, bt_exit, bt_setting;
	Sprite sMeinv, sBackUser, sUserHead, sJf, sRankList, sBack_menu;
	Texture tMeinv, tBackUser, tUserHead, tJf, tRankList, tBack_menu;
	Text text_jb;
	void input_main(Event& e);
	void draw_main();
	//邮件菜单
	int num_mail;
	Sprite sBack_mail, sRedPoint;
	Texture tBack_mail, tRedPoint;
	Button bt_mail_close;
	Mail mail;
	void input_mail(Event& e);
	void draw_mail();
	//玩法菜单
	Sprite sBack_wanfa;
	Texture tBack_wanfa;
	Button bt_wanfa_ok;
	void input_wanfa(Event& e);
	void draw_wanfa();
	//反馈菜单
	Sprite sBack_fankui;
	Texture tBack_fankui;
	Button bt_fankui_close;
	void input_fankui(Event& e);
	void draw_fankui();
	//战绩菜单
	Sprite sBack_zhanji;
	Texture tBack_zhanji;
	Button bt_zhanji_close;
	void input_zhanji(Event& e);
	void draw_zhanji();
	//商店菜单
	Sprite sBack_store;
	Texture tBack_store;
	Button bt_store_close;
	void input_store(Event& e);
	void draw_store();
	//加入游戏
	RoomGUI room_menu;
	Button bt_join, bt_join_exit;
	void input_join(Event& e);
	void draw_join();
};

class GameScene: public Scene //派生出游戏场景类（单机）
{
	friend class Game;
public:
	PukeManager puke_manager;//扑克管理器
	Player human;//玩家
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
	MyClock clock_showCall;//出牌倒计时
	Clock clock_shoot;
	GameScene();
	void Initial_window(RenderWindow* app);
	void Initial_assets();//加载素材
	void Start();//载入场景时执行一次
	void Update();//载入场景后没帧执行一次
	void Input(Event& e);//输入主函数
	void Draw();//绘制主函数
	void SceneClose();//关闭场景
	void player_turn_input(Event& e);//玩家轮流出牌和叫地主
	void input_rhythm(Event& e); //节奏模式输入
private:
	RenderWindow* app;
};

class GameSceneOL :public Scene//联机
{
	friend class Game;
public:
	OLPukeManager puke_manager;//扑克管理器
	Player human_self,human_1,human_2;//玩家
	Sprite sOver, sShoot, sDealBg, sF, sG, sH, sJ;
	Texture tOver[2], tShoot, tDealBg, tF, tG, tH, tJ;
	Text text_score, text_jb, text_over, text_shoot;
	Button bt_over_back, bt_over_restart, bt_exit, bt_setting;
	Sound sound_get;
	SoundBuffer sb_get;
	int score;
	Music mu_over;
	MyClock clock_showCall;//出牌倒计时
	GameSceneOL();
	void Initial_window(RenderWindow* app);
	void Initial_assets();//加载素材
	void Start();//载入场景时执行一次
	void Update();//载入场景后没帧执行一次
	void Input(Event& e);//输入主函数
	void Draw();//绘制主函数
	void SceneClose();//关闭场景
	void player_turn_input(Event& e);//玩家轮流出牌和叫地主
private:
	RenderWindow* app;
	int player_turned_id;
};