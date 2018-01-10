#include "stdafx.h"
#include "Client.h"


Client::Client()
	: m_connected(false)
	, m_serverTimeOffset(0)
	, m_packetSize(std::numeric_limits<Uint8>::max())
{

}
Client::~Client()
{
}
bool Client::connectToServer()
{
	if (!m_connected)
	{
		std::cout << "Connecting to Server........" << std::endl;
		if (SDLNet_Init() != -1)											// Initialize SDLNet
		{
			m_socket = SDLNet_UDP_Open(0);									// (0) Uses an unused port
			SDLNet_ResolveHost(&m_serverIP, m_host.c_str(), m_serverPort);	// Resolves server address

			m_packet = SDLNet_AllocPacket(m_packetSize);					// Set packet size
			m_packet->address.host = m_serverIP.host;						// Set Address & Port
			m_packet->address.port = m_serverIP.port;

			PacketData pData;
			pData << (Uint8)PacketType::Connect;
			if (m_packet->len = pData.getDataSize() > 0)
			{
				memcpy(m_packet->data, pData.getData(), m_packet->len);
				SDLNet_UDP_Send(m_socket, -1, m_packet);
			}

			int currentTime = SDL_GetTicks();

			while (SDL_GetTicks() < (currentTime + TIMEOUT))
			{
				if (SDLNet_UDP_Recv(m_socket, m_packet))
				{
					PacketData pData;
					pData.append(m_packet->data, m_packet->len);

					Uint8 packetType;
					pData >> packetType;

					if (packetType == (Uint8)PacketType::Connect)
					{
						packetHandler(packetType, pData, this);
					}
					m_connected = true;
					//threadPool->addTask(std::bind(&Client::listen, this));			// Starts the listener thread
					return true;
				}
			}

			SDLNet_UDP_Close(m_socket);
			m_socket = NULL;
			std::cout << "Cannot Connect to Server" << std::endl;
		}
	}
	return false;
}

void Client::disconnectFromServer()
{
	if (m_connected)
	{
		m_connected = false;
		SDLNet_UDP_Close(m_socket);
		m_socket = NULL;
	}
}
void Client::listen()
{
	//while (m_connected)
	//{
		if (SDLNet_UDP_Recv(m_socket, m_packet) > 0)
		{
			PacketData pData;
			pData.append(m_packet->data, m_packet->len);

			Uint8 packetType;
			pData >> packetType;

			if (packetType >= (Uint8)PacketType::Connect && packetType < (Uint8)PacketType::OutOfBounds)
			{
				if (packetType == (Uint8)PacketType::Heartbeat)
				{
					PacketData packetData;
					packetData << (Uint8)PacketType::Heartbeat;
					send(packetData);

					Uint32 serverTime;
					pData >> serverTime;
					m_serverTimeOffset = (serverTime - SDL_GetTicks());
				}
				else
				{
					packetHandler(packetType, pData, this);
				}
			}
		}
	//}
}

void Client::send(PacketData packetData)
{
	if (packetData.getDataSize() > 0)
	{
		m_packet->len = packetData.getDataSize();
		memcpy(m_packet->data, packetData.getData(), m_packet->len);
		SDLNet_UDP_Send(m_socket, -1, m_packet);
	}
}
bool Client::isConnected()
{
	return m_connected;
}

int Client::getServerTime()
{
	return m_serverTimeOffset + SDL_GetTicks();
}

void Client::setServerTime(Uint32 serverTime)
{
	m_serverTimeOffset = serverTime - SDL_GetTicks();
}

SDL_mutex * Client::getMutex()
{
	return m_mutex;
}
