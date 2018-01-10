#pragma once
#include "SDL.h"
#include <vector>

enum PacketType
{
	Connect,
	Disconnect,
	EnterRoom,
	LeaveRoom,
	ClientJoinedRoom,
	UpdateRoomData,
	StartGame,
	GameResult,
	Message,
	Update,
	Heartbeat,
	OutOfBounds
};



class PacketData
{
public:
	PacketData();
	~PacketData();

	void clear();
	void append(const void* data, std::size_t sizeInBytes);
	char * getData();
	std::size_t getDataSize() const;

	PacketData& operator >> (Uint8 & data);
	PacketData& operator <<(Uint8 data);
	PacketData& operator >> (Uint16 & data);
	PacketData& operator <<(Uint16 data);
	PacketData& operator >> (Uint32 & data);
	PacketData& operator <<(Uint32 data);
	PacketData& operator >> (std::string & data);
	PacketData& operator << (std::string & data);
	PacketData& operator >> (int & data);
	PacketData& operator << (int data);
	PacketData& operator >> (float& data);
	PacketData& operator <<(float data);
	PacketData& operator >> (double& data);
	PacketData& operator <<(double data);
private:
	std::vector<char> m_data;
	std::size_t m_readPos;
	bool checkSize(std::size_t size);
};

