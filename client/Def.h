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
	illegal = 0, //�Ƿ�
	single, //����
	single_list5,//˳��
	single_list6,
	single_list7,
	single_list8,
	single_list9,
	single_list10,
	single_list11,
	single_list12,
	couple,//����
	couple_list3,//����
	couple_list4,
	couple_list5,
	couple_list6,
	couple_list7,
	couple_list8,
	couple_list9,
	couple_list10,
	three, //��
	three_single,//����1
	three_couple,//����2
	three_list2,//�ɻ��������
	three_list3,
	three_list4,
	three_list5,
	three_list6,
	three_list2_single,//�ɻ������
	three_list3_single,
	three_list4_single,
	three_list5_single,
	three_list2_couple,//�ɻ�������
	three_list3_couple,
	three_list4_couple,
	four_single,//�Ĵ�����
	four_couple,//�Ĵ�����
	bomb, //ը��
	rocket //���
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