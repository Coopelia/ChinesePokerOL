#pragma once
#include<SFML/Network.hpp>
#include<list>
#include<queue>
#include<mutex>
#include"PacketType.hpp"

extern ::std::queue<::sf::Packet> q_sender;//��������Ϣ����
extern Mutex mt_s;//���ʸö��еĻ�����

extern ::std::list<::sf::Packet> q_reciever;//��������Ϣ����
extern Mutex mt_r;//���ʸö��еĻ�����