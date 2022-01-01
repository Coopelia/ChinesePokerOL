#include <SFML/Network.hpp>
#include<string>
#include<iostream>
#include<vector>
#include<sstream>
#include <thread>
#include <pthread.h>
#include <functional>
#include <algorithm>
#include <unordered_map>
#include<list>
#include<queue>
#include<mutex>

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
enum GameSta { Ready = 0, Deal, Call, Play };
enum DEC { NOT = 0, PASS, CHU };

class Player//玩家类
{
public:
	SF sid;//身份
	DEC dec;//抉择状态
	::std::vector<int> hand_card;//手牌数据数组
	int s_call;//叫地主的分值
	bool isCallingDizhu;//是否在叫地主
	bool isMyTime;//是否轮到自己出牌
	bool isWin;
	bool isReady;
	Player();
	void addCard(int x);//添加x到手牌
	void removeCard(int x);//把x移除手牌
	void clear();
};

class Connection
{
public:
	Connection();
	void Stop();
	static void pth_recieve_new_cust(Connection* _this);
	static void pth_update(Connection* _this);
	static void runThreads(Connection* _this);
private:
	uint32_t port_self;
	::sf::TcpListener listener;
	int clientIdTurn;
	bool stop;
	void recieve_new_cust();
};

class Game
{
public:
	Game();
	void start();
	void update();
	void game_logic();
	void setState(bool isPlaying);
	void setID(int id);
	void addPlayer(int id);
	void removePlayer(int id);
	void send_player_info();
	int getNum();
	bool getState();//isPlaying?
	int getID();
	::std::vector<int> getPlayersList();
	friend class Server;
private:
	int ID;//房间号
	bool isPlaying;
	bool isDealing;
	bool isDealDizhu;
	::std::vector<int> player_list;//玩家
	::std::vector<int> dizhuCard;
	::std::vector<int> tempCard;
};

namespace pt
{
	enum MSG_TYPE {
		reNull = 0,
		//客户端发送(服务器接收)
		//request
		reConnect,
		reDisconnect,
		reGetRoomList,
		reCreatRoom,
		reJoinRoom,
		reReady,
		reUnReady,
		reExitRoom,
		reClearDeskCard,

		//date
		daCallDec,
		daChuDec,

		//客户端接收(服务器发送)
		daGameState,//准备，发牌，叫地主，游戏中
		daPlayerStateInfo_Ready,
		daPlayerStateInfo_Call,
		daPlayerStateInfo_Chu,
		daBeishu,
		daDizhuCard,
		daDeskCard,
		daGameOver,
		daRoomList,
		daDealCard,
	};
	// base
	class NetworkEvent
	{
	public:
		NetworkEvent(MSG_TYPE type);
		virtual ~NetworkEvent();

		friend sf::Packet& operator>>(sf::Packet&, NetworkEvent& self);
		friend sf::Packet& operator<<(sf::Packet&, const NetworkEvent& self);

		MSG_TYPE type()const;
	protected:
		MSG_TYPE _type;
	};

	//reConnect
	class ReConnect :public NetworkEvent
	{
	public:
		ReConnect();
	};

	//	reDisconnect
	class ReDisconnect :public NetworkEvent
	{
	public:
		ReDisconnect();
	};

	//	reGetRoomList
	class ReGetRoomList :public NetworkEvent
	{
	public:
		ReGetRoomList();
	};

	//	reCreatRoom
	class ReCreatRoom :public NetworkEvent
	{
	public:
		ReCreatRoom();
	};

	//	reJoinRoom
	class ReJoinRoom :public NetworkEvent
	{
	public:
		ReJoinRoom(int id);

		friend sf::Packet& operator>>(sf::Packet&, ReJoinRoom& self);
		friend sf::Packet& operator<<(sf::Packet&, const ReJoinRoom& self);

		int roomId;
	};

	//	reReady
	class ReReady :public NetworkEvent
	{
	public:
		ReReady();
	};

	//	reUnReady
	class ReUnReady :public NetworkEvent
	{
	public:
		ReUnReady();
	};

	//	reExitRoom
	class ReExitRoom :public NetworkEvent
	{
	public:
		ReExitRoom();
	};

	//reClearDeskCard
	class ReClearDeskCard :public NetworkEvent
	{
	public:
		ReClearDeskCard();
	};

	//	daCallDec
	class DaCallDec :public NetworkEvent
	{
	public:
		DaCallDec();

		friend sf::Packet& operator>>(sf::Packet&, DaCallDec& self);
		friend sf::Packet& operator<<(sf::Packet&, const DaCallDec& self);

		int s_call;
	};

	//	daChuDec
	class DaChuDec :public NetworkEvent
	{
	public:
		DaChuDec();

		friend sf::Packet& operator>>(sf::Packet&, DaChuDec& self);
		friend sf::Packet& operator<<(sf::Packet&, const DaChuDec& self);

		DEC dec;
		::std::vector<int> cards;
	};

	//	daGameState
	class DaGameState :public NetworkEvent
	{
	public:
		DaGameState();

		friend sf::Packet& operator>>(sf::Packet&, DaGameState& self);
		friend sf::Packet& operator<<(sf::Packet&, const DaGameState& self);

		GameSta gsta;
	};

	//	daPlayerStateInfo_Ready
	class DaPlayerStateInfo_Ready :public NetworkEvent
	{
	public:
		DaPlayerStateInfo_Ready();

		friend sf::Packet& operator>>(sf::Packet&, DaPlayerStateInfo_Ready& self);
		friend sf::Packet& operator<<(sf::Packet&, const DaPlayerStateInfo_Ready& self);

		::std::vector<::std::pair<int, bool>> isReady;//first=id,second=isReady(0:false,1:true)
	};

	//	daPlayerStateInfo_Call
	class DaPlayerStateInfo_Call :public NetworkEvent
	{
	public:
		DaPlayerStateInfo_Call();

		friend sf::Packet& operator>>(sf::Packet&, DaPlayerStateInfo_Call& self);
		friend sf::Packet& operator<<(sf::Packet&, const DaPlayerStateInfo_Call& self);

		int player_turned_id;
		int s_call;
	};

	//	daPlayerStateInfo_Chu
	class DaPlayerStateInfo_Chu :public NetworkEvent
	{
	public:
		DaPlayerStateInfo_Chu();

		friend sf::Packet& operator>>(sf::Packet&, DaPlayerStateInfo_Chu& self);
		friend sf::Packet& operator<<(sf::Packet&, const DaPlayerStateInfo_Chu& self);

		int player_turned_id;
		DEC dec;
		::std::vector<::std::pair<::std::pair<int, SF>, ::std::pair<int, ::std::vector<int>>>> playerInfo;//id,sf,num,card
	};

	//daBeishu
	class DaBeishu :public NetworkEvent
	{
	public:
		DaBeishu();

		friend sf::Packet& operator>>(sf::Packet&, DaBeishu& self);
		friend sf::Packet& operator<<(sf::Packet&, const DaBeishu& self);

		int beishu;
	};

	//	daDizhuCard
	class DaDizhuCard :public NetworkEvent
	{
	public:
		DaDizhuCard();

		friend sf::Packet& operator>>(sf::Packet&, DaDizhuCard& self);
		friend sf::Packet& operator<<(sf::Packet&, const DaDizhuCard& self);

		::std::vector<int> cards;
	};

	//	daDeskCard
	class DaDeskCard :public NetworkEvent
	{
	public:
		DaDeskCard();

		friend sf::Packet& operator>>(sf::Packet&, DaDeskCard& self);
		friend sf::Packet& operator<<(sf::Packet&, const DaDeskCard& self);

		::std::vector<int> cards;
	};

	//	daGameOver
	class DaGameOver :public NetworkEvent
	{
	public:
		DaGameOver();

		friend sf::Packet& operator>>(sf::Packet&, DaGameOver& self);
		friend sf::Packet& operator<<(sf::Packet&, const DaGameOver& self);

		int winner_id;
	};

	//	daRoomList
	class DaRoomList :public NetworkEvent
	{
	public:
		DaRoomList();

		friend sf::Packet& operator>>(sf::Packet&, DaRoomList& self);
		friend sf::Packet& operator<<(sf::Packet&, const DaRoomList& self);

		::std::vector<Game*> game;
	};

	//daDealCard
	class DaDealCard :public NetworkEvent
	{
	public:
		DaDealCard();

		friend sf::Packet& operator>>(sf::Packet&, DaDealCard& self);
		friend sf::Packet& operator<<(sf::Packet&, const DaDealCard& self);

		int playerId;
		::std::vector<int> cards;
	};
}

class Customor
{
public:
	Customor();
	~Customor();
	void connect(::sf::IpAddress ip, int port, int id);
	void disconnect();
	int Id();
	::sf::TcpSocket& getSocketIn();
	::sf::TcpSocket& getSocketOut();
	void sendNetworkEvent(::sf::Packet packet);//调用这个函数发送信息
	bool getNetworkEvent(::pt::MSG_TYPE type, ::pt::NetworkEvent*& msg);
	::pt::NetworkEvent* proccessEvent();
	static void pth_send(Customor* _this);
	static void pth_recieve(Customor* _this);
	static void pth_update(Customor* _this);
	static void runThreads(Customor* _this);
	bool isConnected;
	Player player;
private:
	int id;
	int port;

	::sf::IpAddress ip;
	::sf::TcpSocket socket_in;
	::sf::TcpSocket socket_out;

	::std::queue<::sf::Packet> q_sender;//发送者消息队列
	::sf::Mutex mt_s;//访问该队列的互斥锁

	::std::queue<::sf::Packet> q_reciever;//接收者消息队列
	::sf::Mutex mt_r;//访问该队列的互斥锁
};

class Server
{
public:
	Server();
	~Server();
	void Start();
	void Update();
	void Run();
private:
	Connection connector;
	::std::list<Game*> games;
	void update_games();
};

::std::vector<Customor*> clients;
::sf::Mutex mt_c;

Player::Player()
{
	sid = NO;
	dec = NOT;
	s_call = -1;
	isCallingDizhu = false;
	isMyTime = false;
	isWin = false;
	isReady = true;
}

void Player::addCard(int x)
{
	hand_card.push_back(x);
	::std::sort(hand_card.begin(), hand_card.end(), [](int a, int b)->bool {return a <= b; });
}

void Player::removeCard(int x)
{
	::std::vector<int>::iterator itr;
	for (itr = hand_card.begin(); itr != hand_card.end();)
	{
		if ((*itr) == x)
		{
			hand_card.erase(itr);
			return;
		}
		else
			itr++;
	}
}

void Player::clear()
{
	sid = NO;
	dec = NOT;
	hand_card.clear();
	s_call = -1;
	isCallingDizhu = false;
	isMyTime = false;
	isWin = false;
}

namespace pt
{
	NetworkEvent::NetworkEvent(MSG_TYPE type)
	{
		this->_type = type;
	}

	NetworkEvent::~NetworkEvent()
	{
	}

	::pt::MSG_TYPE NetworkEvent::type() const
	{
		return this->_type;
	}

	sf::Packet& operator>>(sf::Packet& packet, NetworkEvent& self)
	{
		return packet;
	}

	sf::Packet& operator<<(sf::Packet& packet, const NetworkEvent& self)
	{
		packet << int(self._type);
		return packet;
	}

	////////////////////// reConnect ////////////////////
	ReConnect::ReConnect() :NetworkEvent(::pt::reConnect)
	{
	}

	////////////////////// reDisconnect ////////////////////
	ReDisconnect::ReDisconnect() : NetworkEvent(::pt::reDisconnect)
	{
	}

	//////////////////// reGetRoomList /////////////////////////////////
	ReGetRoomList::ReGetRoomList() : NetworkEvent(::pt::reGetRoomList)
	{
	}

	////////////////// reCreatRoom //////////////////////
	ReCreatRoom::ReCreatRoom() : NetworkEvent(::pt::reCreatRoom)
	{
	}

	/////////////////////////////// reJoinRoom ////////////////////
	ReJoinRoom::ReJoinRoom(int id) : NetworkEvent(::pt::reJoinRoom)
	{
		this->roomId = id;
	}

	sf::Packet& operator>>(sf::Packet& packet, ReJoinRoom& self)
	{
		packet >> self.roomId;
		return packet;
	}

	sf::Packet& operator<<(sf::Packet& packet, const ReJoinRoom& self)
	{
		packet << self.roomId;
		return packet;
	}

	/////////////////////////////// reReady ////////////////////
	ReReady::ReReady() : NetworkEvent(::pt::reReady)
	{
	}

	/////////////////////////////// reUnReady ////////////////////
	ReUnReady::ReUnReady() : NetworkEvent(::pt::reUnReady)
	{
	}

	/////////////////////////////// reExitRoom ////////////////////
	ReExitRoom::ReExitRoom() : NetworkEvent(::pt::reExitRoom)
	{
	}

	/////////////////////////////// reClearDeskCard ////////////////////
	ReClearDeskCard::ReClearDeskCard() : NetworkEvent(::pt::reClearDeskCard)
	{
	}

	/////////////////////////////// daCallDec ////////////////////
	DaCallDec::DaCallDec() : NetworkEvent(::pt::daCallDec)
	{
	}

	sf::Packet& operator>>(sf::Packet& packet, DaCallDec& self)
	{
		packet >> self.s_call;
		return packet;
	}

	sf::Packet& operator<<(sf::Packet& packet, const DaCallDec& self)
	{
		packet << self.s_call;
		return packet;
	}

	/////////////////////////////// daChuDec ////////////////////
	DaChuDec::DaChuDec() :NetworkEvent(::pt::daChuDec)
	{
	}

	sf::Packet& operator>>(sf::Packet& packet, DaChuDec& self)
	{
		int t;
		packet >> t;
		self.dec = DEC(t);
		while (!packet.endOfPacket())
		{
			packet >> t;
			self.cards.push_back(t);
		}
		return packet;
	}

	sf::Packet& operator<<(sf::Packet& packet, const DaChuDec& self)
	{
		packet << int(self.dec);
		for (int i = 0; i < self.cards.size(); i++)
			packet << self.cards[i];
		return packet;
	}

	/////////////////////////////// daGameState ////////////////////
	DaGameState::DaGameState() :NetworkEvent(::pt::daGameState)
	{
	}

	sf::Packet& operator>>(sf::Packet& packet, DaGameState& self)
	{
		int t;
		packet >> t;
		self.gsta = (GameSta)t;
		return packet;
	}

	sf::Packet& operator<<(sf::Packet& packet, const DaGameState& self)
	{
		packet << int(self.gsta);
		return packet;
	}

	/////////////////////////////// daPlayerStateInfo_Ready ////////////////////
	DaPlayerStateInfo_Ready::DaPlayerStateInfo_Ready() :NetworkEvent(::pt::daPlayerStateInfo_Ready)
	{
	}

	sf::Packet& operator>>(sf::Packet& packet, DaPlayerStateInfo_Ready& self)
	{
		::std::pair<int, bool> t;
		while (!packet.endOfPacket())
		{
			packet >> t.first >> t.second;
			self.isReady.push_back(t);
		}
		return packet;
	}

	sf::Packet& operator<<(sf::Packet& packet, const DaPlayerStateInfo_Ready& self)
	{
		for (int i = 0; i < self.isReady.size(); i++)
			packet << self.isReady[i].first << self.isReady[i].second;
		return packet;
	}

	/////////////////////////////// daPlayerStateInfo_Call ////////////////////
	DaPlayerStateInfo_Call::DaPlayerStateInfo_Call() :NetworkEvent(::pt::daPlayerStateInfo_Call)
	{
	}

	sf::Packet& operator>>(sf::Packet& packet, DaPlayerStateInfo_Call& self)
	{
		packet >> self.player_turned_id >> self.s_call;
		return packet;
	}

	sf::Packet& operator<<(sf::Packet& packet, const DaPlayerStateInfo_Call& self)
	{
		packet << self.player_turned_id << self.s_call;
		return packet;
	}

	/////////////////////////////// daPlayerStateInfo_Chu ////////////////////
	DaPlayerStateInfo_Chu::DaPlayerStateInfo_Chu() :NetworkEvent(::pt::daPlayerStateInfo_Chu)
	{
	}

	sf::Packet& operator>>(sf::Packet& packet, DaPlayerStateInfo_Chu& self)
	{
		packet >> self.player_turned_id;
		int t;
		packet >> t;
		self.dec = DEC(t);
		::std::pair<::std::pair<int, SF>, ::std::pair<int, ::std::vector<int>>> p;
		::std::pair<int, SF> a;
		::std::pair<int, ::std::vector<int>> b;
		while (!packet.endOfPacket())
		{
			packet >> a.first;
			packet >> t;
			a.second = (SF)t;
			packet >> b.first;
			for (int i = 0; i < b.first; i++)
			{
				packet >> t;
				b.second.push_back(t);
			}
			p.first = a;
			p.second = b;
			self.playerInfo.push_back(p);
		}
		return packet;
	}

	sf::Packet& operator<<(sf::Packet& packet, const DaPlayerStateInfo_Chu& self)
	{
		packet << self.player_turned_id;
		packet << int(self.dec);
		for (auto p : self.playerInfo)
		{
			packet << p.first.first << int(p.first.second);
			packet << p.second.first;
			for (int i = 0; i < p.second.first; i++)
				packet << p.second.second[i];
		}
		return packet;
	}

	/////////////////////////////// daBeishu ////////////////////
	DaBeishu::DaBeishu() :NetworkEvent(::pt::daBeishu)
	{
	}

	sf::Packet& operator>>(sf::Packet& packet, DaBeishu& self)
	{
		packet >> self.beishu;
		return packet;
	}

	sf::Packet& operator<<(sf::Packet& packet, const DaBeishu& self)
	{
		packet << self.beishu;
		return packet;
	}

	/////////////////////////////// daDizhuCard ////////////////////
	DaDizhuCard::DaDizhuCard() :NetworkEvent(::pt::daDizhuCard)
	{
	}

	sf::Packet& operator>>(sf::Packet& packet, DaDizhuCard& self)
	{
		int i;
		while (!packet.endOfPacket())
		{
			packet >> i;
			self.cards.push_back(i);
		}
		return packet;
	}

	sf::Packet& operator<<(sf::Packet& packet, const DaDizhuCard& self)
	{
		for (int i = 0; i < self.cards.size(); i++)
			packet << self.cards[i];
		return packet;
	}

	/////////////////////////////// daDeskCard ////////////////////
	DaDeskCard::DaDeskCard() :NetworkEvent(::pt::daDeskCard)
	{
	}

	sf::Packet& operator>>(sf::Packet& packet, DaDeskCard& self)
	{
		int i;
		while (!packet.endOfPacket())
		{
			packet >> i;
			self.cards.push_back(i);
		}
		return packet;
	}

	sf::Packet& operator<<(sf::Packet& packet, const DaDeskCard& self)
	{
		for (int i = 0; i < self.cards.size(); i++)
			packet << self.cards[i];
		return packet;
	}

	/////////////////////////////// daGameOver ////////////////////
	DaGameOver::DaGameOver() :NetworkEvent(::pt::daGameOver)
	{
	}

	sf::Packet& operator>>(sf::Packet& packet, DaGameOver& self)
	{
		packet >> self.winner_id;
		return packet;
	}

	sf::Packet& operator<<(sf::Packet& packet, const DaGameOver& self)
	{
		packet << self.winner_id;
		return packet;
	}

	/////////////////////////////// daRoomList ////////////////////
	DaRoomList::DaRoomList() :NetworkEvent(::pt::daRoomList)
	{
	}

	sf::Packet& operator>>(sf::Packet& packet, DaRoomList& self)
	{
		return packet;
	}

	sf::Packet& operator<<(sf::Packet& packet, const DaRoomList& self)
	{
		int num;
		::std::vector<int> pid;
		for (auto p : self.game)
		{
			num = p->getNum();
			pid = p->getPlayersList();
			packet << p->getID() << p->getState() << num;
			for (int i = 0; i < num; i++)
				packet << clients[pid[i]]->Id();
		}
		return packet;
	}

	/////////////////////////////// daDealCard ////////////////////
	DaDealCard::DaDealCard() :NetworkEvent(::pt::daDealCard)
	{
	}

	sf::Packet& operator>>(sf::Packet& packet, DaDealCard& self)
	{
		packet >> self.playerId;
		int a;
		while (!packet.endOfPacket())
		{
			packet >> a;
			self.cards.push_back(a);
		}
		return packet;
	}

	sf::Packet& operator<<(sf::Packet& packet, const DaDealCard& self)
	{
		packet << self.playerId;
		for (int i = 0; i < self.cards.size(); i++)
			packet << self.cards[i];
		return packet;
	}
}

Customor::Customor()
{
	isConnected = true;
	id = 0;
}

Customor::~Customor()
{

}

void Customor::connect(::sf::IpAddress ip, int port, int id)
{
	this->id = id;
	this->ip = socket_in.getRemoteAddress();
	this->port = port;
	socket_out.connect(ip, port);
	isConnected = true;
	::sf::Packet p;
	p << id;
	socket_out.send(p);
	::std::cout << "客户端已连接，ID：" << id << "\nIP：" << ip << "\n";
}

void Customor::disconnect()
{
	::sf::Packet p;
	int i = 886;
	p << i;
	socket_out.send(p);
	socket_in.disconnect();
	socket_out.disconnect();
	isConnected = false;
	::std::cout << "客户端已断开连接，ID：" << id << "\nIP：" << ip << "\n";
}

::sf::TcpSocket& Customor::getSocketIn()
{
	return socket_in;
}

::sf::TcpSocket& Customor::getSocketOut()
{
	return socket_out;
}

int Customor::Id()
{
	return id;
}

void Customor::sendNetworkEvent(::sf::Packet packet)
{
	mt_s.lock();
	q_sender.push(packet);
	mt_s.unlock();
}

bool Customor::getNetworkEvent(::pt::MSG_TYPE type, ::pt::NetworkEvent*& msg)
{
	if (q_reciever.empty())
		return false;
	::sf::Packet packet;
	int i;
	mt_r.lock();
	packet = q_reciever.front();
	packet >> i;
	::pt::MSG_TYPE _type = static_cast<::pt::MSG_TYPE>(i);
	if (_type == type)
	{
		switch (type)
		{
		case pt::reNull:
			break;
		case pt::reConnect:
			msg = new ::pt::ReConnect();
			break;
		case pt::reDisconnect:
			msg = new ::pt::ReDisconnect();
			break;
		case pt::reGetRoomList:
			msg = new ::pt::ReGetRoomList();
			break;
		case pt::reCreatRoom:
			msg = new ::pt::ReCreatRoom();
			break;
		case pt::reJoinRoom:
			msg = new ::pt::ReJoinRoom(0);
			packet >> (*static_cast<::pt::ReJoinRoom*>(msg));
			break;
		case pt::reReady:
			msg = new ::pt::ReReady();
			break;
		case pt::reUnReady:
			msg = new ::pt::ReUnReady();
			break;
		case pt::reExitRoom:
			msg = new ::pt::ReExitRoom();
			break;
		case pt::reClearDeskCard:
			msg = new ::pt::ReClearDeskCard();
			break;
		case pt::daCallDec:
			msg = new ::pt::DaCallDec();
			packet >> (*static_cast<::pt::DaCallDec*>(msg));
			break;
		case pt::daChuDec:
			msg = new ::pt::DaChuDec();
			packet >> (*static_cast<::pt::DaChuDec*>(msg));
			break;
		case pt::daGameState:
			msg = new ::pt::DaGameState();
			packet >> (*static_cast<::pt::DaGameState*>(msg));
			break;
		case pt::daPlayerStateInfo_Ready:
			msg = new ::pt::DaPlayerStateInfo_Ready();
			packet >> (*static_cast<::pt::DaPlayerStateInfo_Ready*>(msg));
			break;
		case pt::daPlayerStateInfo_Call:
			msg = new ::pt::DaPlayerStateInfo_Call();
			packet >> (*static_cast<::pt::DaPlayerStateInfo_Call*>(msg));
			break;
		case pt::daPlayerStateInfo_Chu:
			msg = new ::pt::DaPlayerStateInfo_Chu();
			packet >> (*static_cast<::pt::DaPlayerStateInfo_Chu*>(msg));
			break;
		case pt::daBeishu:
			msg = new ::pt::DaBeishu();
			packet >> (*static_cast<::pt::DaBeishu*>(msg));
			break;
		case pt::daDizhuCard:
			msg = new ::pt::DaDizhuCard();
			packet >> (*static_cast<::pt::DaDizhuCard*>(msg));
			break;
		case pt::daDeskCard:
			msg = new ::pt::DaDeskCard();
			packet >> (*static_cast<::pt::DaDeskCard*>(msg));
			break;
		case pt::daGameOver:
			msg = new ::pt::DaGameOver();
			packet >> (*static_cast<::pt::DaGameOver*>(msg));
			break;
		case pt::daRoomList:
			msg = new ::pt::DaRoomList();
			packet >> (*static_cast<::pt::DaRoomList*>(msg));
			break;
		case pt::daDealCard:
			msg = new ::pt::DaDealCard();
			packet >> (*static_cast<::pt::DaDealCard*>(msg));
			break;
		default:
			break;
		}
		q_reciever.pop();
	}

	mt_r.unlock();
	if (msg == nullptr)
		return false;
	return true;
}

::pt::NetworkEvent* Customor::proccessEvent()
{
	::pt::NetworkEvent* msg = nullptr;
	mt_r.lock();
	if (!q_reciever.empty())
	{
		::sf::Packet packet = q_reciever.front();
		int i;
		packet >> i;
		::pt::MSG_TYPE type = static_cast<::pt::MSG_TYPE>(i);
		switch (type)
		{
		case pt::reNull:
			break;
		case pt::reConnect:
			msg = new ::pt::ReConnect();
			break;
		case pt::reDisconnect:
			msg = new ::pt::ReDisconnect();
			break;
		case pt::reGetRoomList:
			msg = new ::pt::ReGetRoomList();
			break;
		case pt::reCreatRoom:
			msg = new ::pt::ReCreatRoom();
			break;
		case pt::reJoinRoom:
			msg = new ::pt::ReJoinRoom(0);
			packet >> (*static_cast<::pt::ReJoinRoom*>(msg));
			break;
		case pt::reReady:
			msg = new ::pt::ReReady();
			break;
		case pt::reUnReady:
			msg = new ::pt::ReUnReady();
			break;
		case pt::reExitRoom:
			msg = new ::pt::ReExitRoom();
			break;
		case pt::reClearDeskCard:
			msg = new ::pt::ReClearDeskCard();
			break;
		case pt::daCallDec:
			msg = new ::pt::DaCallDec();
			packet >> (*static_cast<::pt::DaCallDec*>(msg));
			break;
		case pt::daChuDec:
			msg = new ::pt::DaChuDec();
			packet >> (*static_cast<::pt::DaChuDec*>(msg));
			break;
		case pt::daGameState:
			msg = new ::pt::DaGameState();
			packet >> (*static_cast<::pt::DaGameState*>(msg));
			break;
		case pt::daPlayerStateInfo_Ready:
			msg = new ::pt::DaPlayerStateInfo_Ready();
			packet >> (*static_cast<::pt::DaPlayerStateInfo_Ready*>(msg));
			break;
		case pt::daPlayerStateInfo_Call:
			msg = new ::pt::DaPlayerStateInfo_Call();
			packet >> (*static_cast<::pt::DaPlayerStateInfo_Call*>(msg));
			break;
		case pt::daPlayerStateInfo_Chu:
			msg = new ::pt::DaPlayerStateInfo_Chu();
			packet >> (*static_cast<::pt::DaPlayerStateInfo_Chu*>(msg));
			break;
		case pt::daBeishu:
			msg = new ::pt::DaBeishu();
			packet >> (*static_cast<::pt::DaBeishu*>(msg));
			break;
		case pt::daDizhuCard:
			msg = new ::pt::DaDizhuCard();
			packet >> (*static_cast<::pt::DaDizhuCard*>(msg));
			break;
		case pt::daDeskCard:
			msg = new ::pt::DaDeskCard();
			packet >> (*static_cast<::pt::DaDeskCard*>(msg));
			break;
		case pt::daGameOver:
			msg = new ::pt::DaGameOver();
			packet >> (*static_cast<::pt::DaGameOver*>(msg));
			break;
		case pt::daRoomList:
			msg = new ::pt::DaRoomList();
			packet >> (*static_cast<::pt::DaRoomList*>(msg));
			break;
		case pt::daDealCard:
			msg = new ::pt::DaDealCard();
			packet >> (*static_cast<::pt::DaDealCard*>(msg));
			break;
		default:
			break;
		}
		q_reciever.pop();
	}
	mt_r.unlock();
	return msg;
}

void Customor::pth_update(Customor* _this)
{
	while (true)
	{
		if (!_this->isConnected || _this == NULL)
		{
			_this->disconnect();
			return;
		}
	}
}

void Customor::pth_send(Customor* _this)
{
	while (_this->isConnected && _this != NULL)
	{
		_this->mt_s.lock();
		if (!_this->q_sender.empty())
		{
			_this->socket_out.send(_this->q_sender.front());
			_this->q_sender.pop();
		}
		_this->mt_s.unlock();
	}
}

void Customor::pth_recieve(Customor* _this)
{
	/*::sf::SocketSelector socket_selector;
	socket_selector.add(_this->socket_in);*/
	while (_this->isConnected && _this != NULL)
	{
		/*if (not socket_selector.wait(sf::seconds(1)))
			continue;
		if (not socket_selector.isReady(_this->socket_in))
			continue;*/
		::sf::Packet packet;
		if (_this->socket_in.receive(packet) == ::sf::Socket::Done)
			::std::cout << "recieved a message\n";
		::sf::Packet pt = packet;
		int i;
		pt >> i;
		/*	if (i == 10)
				i = 10;*/
		if (i == 886)
		{
			_this->isConnected = false;
			return;
		}
		_this->mt_r.lock();
		_this->q_reciever.push(packet);
		_this->mt_r.unlock();
	}
}

void Customor::runThreads(Customor* _this)
{
	::std::thread th1 = ::std::thread(Customor::pth_send, _this);
	::std::thread th2 = ::std::thread(Customor::pth_recieve, _this);
	::std::thread th3 = ::std::thread(Customor::pth_update, _this);
	th1.detach();
	th2.detach();
	th3.detach();
}

Game::Game()
{
	ID = -1;
	isPlaying = false;
}

void Game::start()
{
	dizhuCard.resize(3, -1);
	tempCard.resize(54);
	::std::vector<int> t(54, 1);
	int n = 0, k;
	srand(time(0));
	while (n < 54)
	{
		k = rand() % 54;
		if (t[k] == 1)
		{
			tempCard[n++] = k;
			t[k] = 0;
		}
	}
	for (int i = 0; i < player_list.size(); i++)
		clients[player_list[i]]->player.clear();
	isDealing = true;
	isDealDizhu = false;
	clients[player_list[0]]->player.isCallingDizhu = true;
}

void Game::update()
{
	//更新游戏状态（准备、发牌等阶段）
	for (int i = 0; i < player_list.size(); i++)
	{
		if (clients[player_list[i]] == nullptr || clients[player_list[i]]->isConnected == false)
			removePlayer(player_list[i]);
	}

	int n = 0;
	for (int i = 0; i < player_list.size(); i++)
		if (clients[player_list[i]]->player.isReady)
			n++;
	if (isPlaying && n < 3)
	{
		isPlaying = false;
		::pt::DaGameState dgs;
		dgs.gsta = Ready;
		::sf::Packet packet;
		packet << static_cast<int>(dgs.type()) << dgs;
		for (int i = 0; i < player_list.size(); i++)
			clients[player_list[i]]->sendNetworkEvent(packet);
		::std::cout << "send game state : ready\n";
	}
	else if (!isPlaying && n == 3)
	{
		isPlaying = true;
		start();
		::pt::DaGameState dgs;
		dgs.gsta = Deal;
		::sf::Packet packet;
		packet << static_cast<int>(dgs.type()) << dgs;
		for (int i = 0; i < player_list.size(); i++)
			clients[player_list[i]]->sendNetworkEvent(packet);
		::std::cout << "send game state : deal\n";
	}

	//处理客户端请求
	for (int i = 0; i < player_list.size(); i++)
	{
		::pt::NetworkEvent* e = clients[player_list[i]]->proccessEvent();
		if (e == nullptr)
			continue;
		::pt::MSG_TYPE type = e->type();
		switch (type)
		{
		case pt::reNull:
			break;
		case pt::reConnect:
			break;
		case pt::reDisconnect:
			clients[player_list[i]]->disconnect();
			removePlayer(player_list[i]);
			break;
		case pt::reGetRoomList:
			break;
		case pt::reCreatRoom:
			break;
		case pt::reJoinRoom:
			break;
		case pt::reReady:
		{
			clients[player_list[i]]->player.isReady = true;
			::pt::DaPlayerStateInfo_Ready dsr;
			for (int j = 0; j < player_list.size(); j++)
			{
				::std::pair<int, bool> a;
				a.first = clients[player_list[j]]->Id();
				a.second = clients[player_list[j]]->player.isReady;
				dsr.isReady.push_back(a);
			}
			::sf::Packet packet;
			packet << static_cast<int>(dsr.type()) << dsr;
			for (int j = 0; j < player_list.size(); j++)
				clients[player_list[j]]->sendNetworkEvent(packet);
			::std::cout << "get ready and send player state information\n";
			break;
		}
		case pt::reUnReady:
		{
			clients[player_list[i]]->player.isReady = false;
			::pt::DaPlayerStateInfo_Ready dsr;
			for (int j = 0; j < player_list.size(); j++)
			{
				::std::pair<int, bool> a;
				a.first = clients[player_list[j]]->Id();
				a.second = clients[player_list[j]]->player.isReady;
				dsr.isReady.push_back(a);
			}
			::sf::Packet packet;
			packet << static_cast<int>(dsr.type()) << dsr;
			for (int j = 0; j < player_list.size(); j++)
				clients[player_list[j]]->sendNetworkEvent(packet);
			::std::cout << "get unready and send player state information\n";
			break;
		}
		case pt::reExitRoom:
			removePlayer(player_list[i]);
			break;
			//清空出牌区
		case pt::reClearDeskCard:
		{
			::pt::DaDeskCard ddc;
			ddc.cards.resize(0);
			::sf::Packet packet;
			packet << static_cast<int>(ddc.type()) << ddc;
			for (int j = 0; j < player_list.size(); j++)
				clients[player_list[j]]->sendNetworkEvent(packet);
			break;
		}
		//叫分
		case pt::daCallDec:
		{
			::std::cout << "recieve: call_dec\n";
			clients[player_list[i]]->player.isCallingDizhu = false;
			clients[player_list[i]]->player.s_call = static_cast<::pt::DaCallDec*>(e)->s_call;
			::pt::DaPlayerStateInfo_Call dpsc;
			dpsc.player_turned_id = clients[player_list[i]]->Id();
			dpsc.s_call = clients[player_list[i]]->player.s_call;
			::sf::Packet packet;
			packet << static_cast<int>(dpsc.type()) << dpsc;
			for (int j = 0; j < player_list.size(); j++)
				clients[player_list[j]]->sendNetworkEvent(packet);
			if (i < player_list.size() - 1)
			{
				dpsc.player_turned_id = clients[player_list[i + 1]]->Id();
				dpsc.s_call = clients[player_list[i + 1]]->player.s_call;
				clients[player_list[i + 1]]->player.isCallingDizhu = true;
			}
			else
			{
				dpsc.player_turned_id = clients[player_list[0]]->Id();
				clients[player_list[0]]->player.isCallingDizhu = true;
				dpsc.s_call = clients[player_list[0]]->player.s_call;
			}
			packet.clear();
			packet << static_cast<int>(dpsc.type()) << dpsc;
			for (int j = 0; j < player_list.size(); j++)
				clients[player_list[j]]->sendNetworkEvent(packet);
			break;
		}
		//出牌
		case pt::daChuDec:
		{
			clients[player_list[i]]->player.dec = static_cast<::pt::DaChuDec*>(e)->dec;
			if (clients[player_list[i]]->player.dec == CHU)
			{
				::std::vector<int> cards = static_cast<::pt::DaChuDec*>(e)->cards;
				::pt::DaDeskCard dc;
				dc.cards = cards;
				::sf::Packet packet;
				packet << static_cast<int>(dc.type()) << dc;
				for (int j = 0; j < player_list.size(); j++)
					clients[player_list[j]]->sendNetworkEvent(packet);
				for (int j = 0; j < cards.size(); j++)
					clients[player_list[i]]->player.removeCard(cards[j]);
			}
			send_player_info();
			clients[player_list[i]]->player.isMyTime = false;
			if (i + 1 != player_list.size())
			{
				clients[player_list[i + 1]]->player.dec = NOT;
				clients[player_list[i + 1]]->player.isMyTime = true;
			}
			else
			{
				clients[player_list[0]]->player.dec = NOT;
				clients[player_list[0]]->player.isMyTime = true;
			}
			send_player_info();
			break;
		}
		case pt::daGameState:
			break;
		case pt::daPlayerStateInfo_Ready:
			break;
		case pt::daPlayerStateInfo_Call:
			break;
		case pt::daPlayerStateInfo_Chu:
			break;
		case pt::daBeishu:
			break;
		case pt::daDizhuCard:
			break;
		case pt::daDeskCard:
			break;
		case pt::daGameOver:
			break;
		case pt::daRoomList:
			break;
		case pt::daDealCard:
			break;
		default:
			break;
		}
	}

	//游戏逻辑
	if (isPlaying)
		game_logic();
}

void Game::send_player_info()
{
	int t = -1;
	for (int i = 0; i < player_list.size(); i++)
	{
		if (clients[player_list[i]]->player.isMyTime)
			t = i;
	}
	::pt::DaPlayerStateInfo_Chu dsc;
	if (t != -1)
	{
		dsc.player_turned_id = clients[player_list[t]]->Id();
		dsc.dec = clients[player_list[t]]->player.dec;
	}
	else
	{
		dsc.player_turned_id = -1;
		dsc.dec = NOT;
	}
	for (int i = 0; i < player_list.size(); i++)
	{
		::std::pair<::std::pair<int, SF>, ::std::pair<int, ::std::vector<int>>> c;
		c.first.first = clients[player_list[i]]->Id();
		c.first.second = clients[player_list[i]]->player.sid;
		c.second.first = clients[player_list[i]]->player.hand_card.size();
		c.second.second = clients[player_list[i]]->player.hand_card;
		dsc.playerInfo.push_back(c);
	}
	::sf::Packet packet;
	packet << static_cast<int>(dsc.type()) << dsc;
	for (int i = 0; i < player_list.size(); i++)
		clients[player_list[i]]->sendNetworkEvent(packet);
}

void Game::game_logic()
{
	if (isDealing)
	{
		::pt::DaGameState dgs;
		dgs.gsta = Deal;
		::sf::Packet packet;
		packet << static_cast<int>(dgs.type()) << dgs;
		for (int i = 0; i < player_list.size(); i++)
			clients[player_list[i]]->sendNetworkEvent(packet);

		int t = 0;
		while (tempCard.size() > 3)
		{
			/*::pt::DaDealCard ddc;
			ddc.playerId = clients[player_list[t]]->Id();
			ddc.cards.push_back(tempCard[tempCard.size() - 1]);*/
			clients[player_list[t]]->player.addCard(tempCard[tempCard.size() - 1]);
			tempCard.pop_back();
			/*::sf::Packet packet;
			packet << static_cast<int>(ddc.type()) << ddc;
			for (int i = 0; i < player_list.size(); i++)
				clients[player_list[i]]->sendNetworkEvent(packet);*/
			t = (t + 1) % 3;
			send_player_info();
		}

		::pt::DaDizhuCard dc;
		::std::sort(tempCard.begin(), tempCard.end(), [](int a, int b)->bool {return a < b; });
		dc.cards = tempCard;
		dizhuCard = tempCard;
		tempCard.clear();
		packet.clear();
		packet << static_cast<int>(dc.type()) << dc;
		for (int i = 0; i < player_list.size(); i++)
			clients[player_list[i]]->sendNetworkEvent(packet);
		isDealing = false;
		isDealDizhu = true;
		clients[player_list[0]]->player.isCallingDizhu = true;
		clients[player_list[0]]->player.s_call = -1;

		::pt::DaPlayerStateInfo_Call dpsc;
		dpsc.player_turned_id = clients[player_list[0]]->Id();
		dpsc.s_call = clients[player_list[0]]->player.s_call;
		packet.clear();
		packet << static_cast<int>(dpsc.type()) << dpsc;
		for (int j = 0; j < player_list.size(); j++)
			clients[player_list[j]]->sendNetworkEvent(packet);

		dgs.gsta = Call;
		packet.clear();
		packet << static_cast<int>(dgs.type()) << dgs;
		for (int i = 0; i < player_list.size(); i++)
			clients[player_list[i]]->sendNetworkEvent(packet);
	}
	else if (isDealDizhu)
	{
		if (clients[player_list[0]]->player.s_call != -1 && clients[player_list[1]]->player.s_call != -1 && clients[player_list[2]]->player.s_call != -1)
		{
			int t = 0;
			for (int i = 0; i < player_list.size(); i++)
			{
				clients[player_list[i]]->player.isCallingDizhu = false;
				if (clients[player_list[i]]->player.s_call > clients[player_list[t]]->player.s_call)
					t = i;
			}

			clients[player_list[t]]->player.sid = DIZHU;
			clients[player_list[t]]->player.isMyTime = true;
			clients[player_list[(t + 1) % 3]]->player.sid = NONM;
			clients[player_list[(t + 2) % 3]]->player.sid = NONM;
			/*::pt::DaDealCard ddc;
			ddc.playerId = clients[player_list[t]]->Id();*/
			for (int i = 0; i < 3; i++)
			{
				//ddc.cards.push_back(dizhuCard[i]);
				clients[player_list[t]]->player.addCard(dizhuCard[i]);
			}
			::sf::Packet packet;
			/*packet << static_cast<int>(ddc.type()) << ddc;
			for (int i = 0; i < player_list.size(); i++)
				clients[player_list[i]]->sendNetworkEvent(packet);*/
			isDealDizhu = false;
			::pt::DaGameState dgs;
			dgs.gsta = Play;
			packet.clear();
			packet << static_cast<int>(dgs.type()) << dgs;
			for (int i = 0; i < player_list.size(); i++)
				clients[player_list[i]]->sendNetworkEvent(packet);

			send_player_info();
		}
	}
	else
	{
		//判断游戏是否结束
		int t = -1;
		for (int i = 0; i < player_list.size(); i++)
		{
			if (clients[player_list[i]]->player.hand_card.size() == 0)
				t = i;
		}
		if (t != -1)
		{
			isPlaying = false;
			::pt::DaGameOver dgo;
			dgo.winner_id = clients[player_list[t]]->Id();
			::sf::Packet packet;
			packet << static_cast<int>(dgo.type()) << dgo;
			for (int i = 0; i < player_list.size(); i++)
			{
				clients[player_list[i]]->player.isReady = false;
				clients[player_list[i]]->sendNetworkEvent(packet);
			}
		}
	}
}

void Game::setState(bool isPlaying)
{
	this->isPlaying = isPlaying;
}

void Game::setID(int id)
{
	this->ID = id;
}

void Game::addPlayer(int id)
{
	player_list.push_back(id);
	::pt::DaGameState dgs;
	if (player_list.size() == 3)
		dgs.gsta = Deal;
	else
		dgs.gsta = Ready;
	::sf::Packet packet;
	packet << static_cast<int>(dgs.type()) << dgs;
	for (int i = 0; i < player_list.size(); i++)
		clients[player_list[i]]->sendNetworkEvent(packet);
	::pt::DaPlayerStateInfo_Ready dpsr;
	for (int i = 0; i < player_list.size(); i++)
		dpsr.isReady.push_back(::std::pair<int, bool>(clients[player_list[i]]->Id(), clients[player_list[i]]->player.isReady));
	packet.clear();
	packet << static_cast<int>(dpsr.type()) << dpsr;
	for (int i = 0; i < player_list.size(); i++)
		clients[player_list[i]]->sendNetworkEvent(packet);
}

void Game::removePlayer(int id)
{
	::std::vector<int>::iterator itr;
	for (itr = player_list.begin(); itr != player_list.end();)
	{
		if (id == *itr)
		{
			player_list.erase(itr);
			return;
		}
		itr++;
	}
}

int Game::getNum()
{
	return this->player_list.size();
}

bool Game::getState()
{
	return this->isPlaying;
}

int Game::getID()
{
	return this->ID;
}

::std::vector<int> Game::getPlayersList()
{
	return this->player_list;
}

Connection::Connection()
{
	port_self = 5688;
	listener.listen(port_self);
	clientIdTurn = 1;
	stop = false;
	::std::cout << "host id: " << ::sf::IpAddress::getLocalAddress() << ::std::endl;
}

void Connection::Stop()
{
	this->stop = true;
	::std::vector<Customor*>::iterator itr;
	mt_c.lock();
	for (itr = clients.begin(); itr != clients.end();)
	{
		(*itr)->disconnect();
		itr = clients.erase(itr);
	}
	mt_c.unlock();
}

void Connection::recieve_new_cust()
{
	Customor* c = new Customor();
	if (listener.accept(c->getSocketIn()) == ::sf::Socket::Done)
	{
		::std::cout << "接收到一个新连接: " << c->getSocketIn().getRemoteAddress() << "\n";
		::sf::Packet p;
		c->getSocketIn().receive(p);
		int port;
		p >> port;
		c->connect(c->getSocketIn().getRemoteAddress(), port, clientIdTurn++);
		mt_c.lock();
		c->runThreads(*&c);
		clients.push_back(c);
		mt_c.unlock();
	}
}

void Connection::pth_recieve_new_cust(Connection* _this)
{
	while (!_this->stop)
	{
		_this->recieve_new_cust();
	}
}

void Connection::pth_update(Connection* _this)
{
	while (!_this->stop)
	{
		::std::vector<Customor*>::iterator itr;
		mt_c.lock();
		for (itr = clients.begin(); itr != clients.end();)
		{
			if (!(*itr)->isConnected)
			{
				itr = clients.erase(itr);
				_this->clientIdTurn--;
			}
			else
				itr++;
		}
		mt_c.unlock();
	}
}

void Connection::runThreads(Connection* _this)
{
	::std::thread th1 = ::std::thread(Connection::pth_recieve_new_cust, _this);
	::std::thread th2 = ::std::thread(Connection::pth_update, _this);
	th1.detach();
	th2.detach();
}

Server::Server()
{
	::std::cout << "server is running, waiting for the customor.\n";
}

Server::~Server()
{
	for (auto p : games)
	{
		delete p;
	}
	for (auto p : clients)
	{
		delete p;
	}
}

void Server::Start()
{
	connector.runThreads(&connector);
	::std::list<Game*>::iterator itr;
	for (itr = games.begin(); itr != games.end(); itr++)
		(*itr)->start();
}

void Server::update_games()
{
	//遍历每个房间，更新数据
	::std::list<Game* >::iterator itr;
	for (itr = games.begin(); itr != games.end();)
	{
		if ((*itr)->player_list.size() == 0)
			itr = games.erase(itr);
		else
		{
			(*itr)->update();
			itr++;
		}
	}
}

void Server::Update()
{
	mt_c.lock();
	for (int i = 0; i < clients.size(); i++)
	{
		//玩家有没有发送创建（加入）房间的请求
		::pt::NetworkEvent* rcr = nullptr;
		if (clients[i]->getNetworkEvent(::pt::reCreatRoom, rcr))//创建房间
		{
			::std::cout << "create a game\n";
			Game* game = new Game();
			game->setID(games.size() + 1);
			game->addPlayer(i);
			games.push_back(game);
			::pt::DaPlayerStateInfo_Ready dpsr;
			dpsr.isReady.push_back(::std::pair<int, bool>(clients[i]->Id(), true));
			::sf::Packet packet;
			packet << static_cast<int>(dpsr.type()) << dpsr;
			clients[i]->sendNetworkEvent(packet);
			delete rcr;
		}
		if (clients[i]->getNetworkEvent(::pt::reGetRoomList, rcr))//获取房间列表
		{
			::std::cout << "send game list\n";
			::pt::DaRoomList drl;
			::sf::Packet packet;
			::std::list<Game*>::iterator it;
			for (it = games.begin(); it != games.end(); it++)
				drl.game.push_back((*it));
			packet << static_cast<int>(drl.type()) << drl;
			clients[i]->sendNetworkEvent(packet);
			delete rcr;
		}
		//加入房间
		::pt::NetworkEvent* rjr = nullptr;
		if (clients[i]->getNetworkEvent(::pt::reJoinRoom, rjr))
		{
			::std::list<Game*>::iterator it;
			for (it = games.begin(); it != games.end(); it++)
			{
				if ((*it)->getID() == static_cast<::pt::ReJoinRoom*>(rjr)->roomId)
				{
					::pt::ReJoinRoom re(0);
					re.roomId = (*it)->getID();
					::sf::Packet pt;
					pt << static_cast<int>(re.type()) << re;
					clients[i]->sendNetworkEvent(pt);
					(*it)->addPlayer(i);
					break;
				}
			}
			delete rjr;
		}
	}
	mt_c.unlock();
	update_games();
}

void Server::Run()
{
	Start();
	while (true)
	{
		Update();
	}
}

int main()
{
	Server server;
	server.Run();

	return 0;
}