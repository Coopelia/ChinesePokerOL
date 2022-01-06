#pragma once
#include "Def.h"

class Game;

namespace pt
{
	enum MSG_TYPE {
		reNull=0,
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

		::std::vector<::std::pair<int,bool>> isReady;//first=id,second=isReady(0:false,1:true)
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
	class DaDeskCard:public NetworkEvent
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