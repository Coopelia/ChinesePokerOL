#include "MsgQueue.h"

::std::queue<::sf::Packet> q_sender; //发送者消息队列
::std::list<::sf::Packet> q_reciever; //接收者消息队列

Mutex mt_s;
Mutex mt_r;