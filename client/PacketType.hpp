#include "Def.h"
#include "Room.h"
#include "Player.h"

namespace pt
{
	enum MSG_TYPE {
		//客户端发送(服务器接收)
		//request
		reConnect=0,
		reDisconnect,
		reGetRoomList,
		reCreatRoom,
		reJoinRoom,
		reReady,
		reUnReady,
		reExitRoom,

		//date
		daCallDec,
		daChuDec,

		//客户端接收(服务器发送)
		daGameState,//准备，发牌，叫地主，游戏中
		daPlayerStateInfo_Ready,
		daPlayerStateInfo_Call,
		daPlayerStateInfo_Chu,
		daPlayerSelfCard,
		daDizhuCard,
		daDeskCard,
		daGameOver,
		daRoomList,
	};
	// base
	class NetworkEvent
	{
	public:
		NetworkEvent(MSG_TYPE type);
		virtual ~NetworkEvent();
		MSG_TYPE type()const;
	protected:
		const MSG_TYPE type;
	};

	class Connected : public NetworkEvent
	{
	public:
		Connected();
	};

	class Disconnected : public NetworkEvent
	{
	public:
		Disconnected();
	};

	class LogOut : public NetworkEvent
	{
	public:
		LogOut();
		void setMessage(::std::string s);
		::std::string getMessage();
	private:
		::std::string msg;
	};

	class GetListGame : public NetworkEvent
	{
	public:
		GetListGame();
		const ::std::vector<Room> getRoomList()const;
	private:
		::std::vector<Room> room;
	};

	class SetListGame : public NetworkEvent
	{
	public:
		SetListGame();

		friend sf::Packet& operator>>(sf::Packet&, SetListGame& self);
		friend sf::Packet& operator<<(sf::Packet&, const SetListGame& self);

		void add(Room room);
		const ::std::vector<Room>& getRoom() const;
	private:
		::std::vector<Room> room;
	};

	class CreateGame : public NetworkEvent
	{
	public:
		CreateGame();
	};

	class JoinGame : public NetworkEvent
	{
	public:
		JoinGame(int roomId);

		int GetRoomId()const;

		friend sf::Packet& operator>>(sf::Packet&, JoinGame& self);
		friend sf::Packet& operator<<(sf::Packet&, const JoinGame& self);

	private:
		int roomId;
	};

	class JoinGameOk : public NetworkEvent
	{
	public:
		JoinGameOk(int roomId);

		friend sf::Packet& operator>>(sf::Packet&, JoinGameOk& self);
		friend sf::Packet& operator<<(sf::Packet&, const JoinGameOk& self);

		::std::vector<PlayerInfo> getPlayerList();
	private:
		int roomId;
		::std::vector<PlayerInfo> playerList;
	};

	class JoinGameReject : public NetworkEvent
	{
	public:
		JoinGameReject(int roomId);

		int GetRoomId()const;

		friend sf::Packet& operator>>(sf::Packet&, JoinGameReject& self);
		friend sf::Packet& operator<<(sf::Packet&, const JoinGameReject& self);
	private:
		int roomId;
	};

	//in game
	class UpdateSF :public NetworkEvent
	{
	public:
		UpdateSF(SF sfId);
	private:
		SF sfId;//玩家身份（地主or农民）
	};
	
	class UpdateDeskCard :public NetworkEvent
	{
	public:
		UpdateDeskCard();

		void setDeskCard(::std::vector<int>& cards);
		void setDizhuCard(::std::vector<int>& cards);
		
		::std::vector<int> getDeskCard();
		::std::vector<int> getDizhuCard();

	private:
		::std::vector<int> desk_card;
		::std::vector<int> dizhu_card;
	};

	class UpdateHandCard :public NetworkEvent
	{
	public:
		UpdateHandCard();

		void setHandCard(::std::vector<int>& cards);

		::std::vector<int> getHandCard();

	private:
		::std::vector<int> hand_card;
	};

	class UpdateState :public NetworkEvent
	{
	public:
		UpdateState();

		PlayerInfo getPlayerInfo();

	private:
		PlayerInfo player_info;
	};

	class UpdateTimeChuPai :public NetworkEvent
	{
	public:
		UpdateTimeChuPai();

	};

	class UpdateBeiShu :public NetworkEvent
	{
	public:
		UpdateBeiShu();


	};

	class UpdateGameOver :public NetworkEvent
	{
	public:
		UpdateGameOver();

	};

	//叫地主阶段
	class UpdateTimeCall :public NetworkEvent
	{
	public:
		UpdateTimeCall();

	};

	class UpdateScoreCall :public NetworkEvent
	{
	public:
		UpdateScoreCall();


	};

	class UpdateResultCall :public NetworkEvent
	{
	public:
		UpdateResultCall();


	};
}