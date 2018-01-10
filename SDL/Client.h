#pragma once
#include <iostream>
#include <SDL_net.h>
#include "SDL.h"
#include "ThreadPool.h"
#include "PacketData.h"

class Client
{
public:
	Client();
	~Client();

	template<class T>
	void setup(void(T::*handlePacket)(Uint8&, PacketData&, Client*), T* instance)
	{
		packetHandler = std::bind(handlePacket, instance, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	}
	std::function<void(Uint8&, PacketData&, Client*)> packetHandler;

	bool connectToServer();// TODO: Connect to server
	void disconnectFromServer();			// TODO: Remove to server
	void listen();							// Listen for messages
	void send(PacketData packetData);		// Send for messages
	bool isConnected();						// Is Client Connected
	int getServerTime();
	void setServerTime(Uint32 serverTime);
	SDL_mutex * getMutex();
private:
	bool m_connected;
	Uint32 m_serverTimeOffset;
	Uint8 m_packetSize;
	SDL_mutex * m_mutex;

	// SDLNet Vars
	////////////////////////////////////////////////
	UDPsocket m_socket;
	UDPpacket * m_packet;
	IPaddress m_serverIP;



	std::string m_host = "149.153.106.150";


	Uint16 m_serverPort = 5228;

	const int TIMEOUT = 5000;

};

