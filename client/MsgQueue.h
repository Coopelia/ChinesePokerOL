#pragma once
#include<SFML/Network.hpp>
#include<list>
#include<queue>
#include<mutex>
#include"PacketType.hpp"

extern ::std::queue<::sf::Packet> q_sender;//发送者消息队列
extern Mutex mt_s;//访问该队列的互斥锁

extern ::std::list<::sf::Packet> q_reciever;//接收者消息队列
extern Mutex mt_r;//访问该队列的互斥锁