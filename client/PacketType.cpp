#include "PacketType.hpp"

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
		packet << sf::Uint8(self._type);
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
	ReJoinRoom::ReJoinRoom() : NetworkEvent(::pt::reJoinRoom)
	{
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

	sf::Packet& operator<<(sf::Packet& packet, DaCallDec& self)
	{
		packet << ::sf::Uint8(self.s_call);
		return packet;
	}

	/////////////////////////////// daChuDec ////////////////////
	DaChuDec::DaChuDec() :NetworkEvent(::pt::daChuDec)
	{
	}

	sf::Packet& operator>>(sf::Packet& packet, DaChuDec& self)
	{
		::sf::Uint8 t;
		packet >> t;
		self.dec = DEC(t);
		int i;
		while (!packet.endOfPacket())
		{
			packet >> i;
			self.cards.push_back(i);
		}
		return packet;
	}

	sf::Packet& operator<<(sf::Packet& packet, DaChuDec& self)
	{
		packet << ::sf::Uint8(self.dec);
		for (auto p : self.cards)
			packet << p;
		return packet;
	}

	/////////////////////////////// daGameState ////////////////////
	DaGameState::DaGameState() :NetworkEvent(::pt::daGameOver)
	{
	}

	sf::Packet& operator>>(sf::Packet& packet, DaGameState& self)
	{
		::sf::Uint8 t;
		packet >> t;
		self.gsta = (GameSta)t;
		return packet;
	}

	sf::Packet& operator<<(sf::Packet& packet, DaGameState& self)
	{
		packet << ::sf::Uint8(self.gsta);
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

	sf::Packet& operator<<(sf::Packet& packet, DaPlayerStateInfo_Ready& self)
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

	sf::Packet& operator<<(sf::Packet& packet, DaPlayerStateInfo_Call& self)
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
		::sf::Uint8 t;
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

	sf::Packet& operator<<(sf::Packet& packet, DaPlayerStateInfo_Chu& self)
	{
		packet << self.player_turned_id;
		packet << ::sf::Uint8(self.dec);
		for (auto p : self.playerInfo)
		{
			packet << p.first.first << p.first.second;
			packet << p.second.first;
			for (int i = 0; i < p.second.second.size(); i++)
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

	sf::Packet& operator<<(sf::Packet& packet, DaBeishu& self)
	{
		packet << self.beishu;
		return packet;
	}

	/////////////////////////////// daDizhuCard ////////////////////
	DaDizhuCard::DaDizhuCard() :NetworkEvent(::pt::daDeskCard)
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

	sf::Packet& operator<<(sf::Packet& packet, DaDizhuCard& self)
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

	sf::Packet& operator<<(sf::Packet& packet, DaDeskCard& self)
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

	sf::Packet& operator<<(sf::Packet& packet, DaGameOver& self)
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
		Room room;
		int t;
		bool f;
		while (!packet.endOfPacket())
		{
			packet >> t;
			room.setID(t);
			packet >> f;
			room.setState(f);
			packet >> t;
			room.setNum(t);
			for (int i = 0; i < room.getNum(); i++)
			{
				packet >> t;
				room.addPlayer(t);
			}
			self.room.push_back(room);
		}
		return packet;
	}

	sf::Packet& operator<<(sf::Packet& packet, DaRoomList& self)
	{
		int num;
		::std::vector<int> pid;
		for (auto p : self.room)
		{
			num = p.getNum();
			pid = p.getPlayersId();
			packet << p.getID() << p.getState() << num;
			for (int i = 0; i < num; i++)
				packet << pid[i];
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

	sf::Packet& operator<<(sf::Packet& packet, DaDealCard& self)
	{
		packet << self.playerId;
		for (int i = 0; i < self.cards.size(); i++)
			packet << self.cards[i];
		return packet;
	}
}