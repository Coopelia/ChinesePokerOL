#include "MsgQueue.h"

::std::queue<::sf::Packet> q_sender; //��������Ϣ����
::std::list<::sf::Packet> q_reciever; //��������Ϣ����

Mutex mt_s;
Mutex mt_r;