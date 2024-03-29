#include "PacketType.h"

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
		while (!packet.endOfPacket())
		{
			::std::pair<::std::pair<int, SF>, ::std::pair<int, ::std::vector<int>>> p;
			packet >> p.first.first;
			packet >> t;
			p.first.second = (SF)t;
			packet >> p.second.first;
			for (int i = 0; i < p.second.first; i++)
			{
				packet >> t;
				p.second.second.push_back(t);
			}
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
			packet << p.first.first <<int(p.first.second);
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
		Room room;
		int t, n;
		bool f;
		while (!packet.endOfPacket())
		{
			packet >> t;
			room.setID(t);
			packet >> f;
			room.setState(f);
			packet >> n;
			for (int i = 0; i < n; i++)
			{
				packet >> t;
				room.addPlayer(t);
			}
			self.room.push_back(room);
		}
		return packet;
	}

	sf::Packet& operator<<(sf::Packet& packet, const DaRoomList& self)
	{
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