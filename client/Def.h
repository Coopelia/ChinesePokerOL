#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include<string>
#include<windows.h>
#include<iostream>
#include<vector>
#include<sstream>
#include <thread>
#include <functional>
#include <algorithm>
#include <unordered_map>
#define WIDTH 1280
#define HEIGHT 720
using namespace sf;

enum SF { NO = 0, DIZHU, NONM };
enum PukeType { 
	illegal = 0, //非法
	single, //单牌
	single_list5,//顺子
	single_list6,
	single_list7,
	single_list8,
	single_list9,
	single_list10,
	single_list11,
	single_list12,
	couple,//对子
	couple_list3,//连对
	couple_list4,
	couple_list5,
	couple_list6,
	couple_list7,
	couple_list8,
	couple_list9,
	couple_list10,
	three, //三
	three_single,//三带1
	three_couple,//三带2
	three_list2,//飞机不带翅膀
	three_list3,
	three_list4,
	three_list5,
	three_list6,
	three_list2_single,//飞机带翅膀单
	three_list3_single,
	three_list4_single,
	three_list5_single,
	three_list2_couple,//飞机带翅膀对
	three_list3_couple,
	three_list4_couple,
	four_single,//四带二单
	four_couple,//四带二对
	bomb, //炸弹
	rocket //火箭
};
enum DEC { NOT = 0, PASS, CHU };
extern int vol_bgm, vol_sound;
extern int jb;
extern int px1, py1, px2, py2;
extern bool isRhythm;
//extern class Card;
//extern class Puke;
//extern class Button;
//extern class StartScene;
//extern class GameScene;
//extern class Game;
//extern class MyClock;
//extern class User;
//extern class Player;
//extern class AI;
//extern class PukeManager;
//extern class MouseRectIn;
//extern class Mail;
//extern class GunCharm;

extern bool isLoading;
void LoadAnim(RenderWindow* app);