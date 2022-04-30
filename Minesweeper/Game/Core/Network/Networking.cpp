#include "Networking.h"

Networking* Networking::s_Instance = nullptr;

void Networking::ProcessMessages()
{
	while (!netEngine.deqQueue.empty())
	{
		message m = netEngine.deqQueue.front();
		netEngine.deqQueue.pop_front();
		switch (m.header.id)
		{
		case (int)GameMsg::Game_UpdatePlayer:
		{
			vec3f* pos = reinterpret_cast<vec3f*>(m.body);
			entity.SetPlayerDescription((uint32_t)m.header.socketId, { *pos });

			netEngine.SendSomeDataExcept(m, m.header.socketId);

			break;
		}
		case (int)GameMsg::Server_AssignPlayerID:
		{
			netEngine.clientID = m.header.socketId;
			break;
		}
		}
	}
	entity.Draw();
}

void Networking::SendMessageToAll(message& msg)
{
	netEngine.SendSomeData(msg);
}

void Networking::SendMessageExcept(message& msg, int excludedID)
{
	netEngine.SendSomeDataExcept(msg, excludedID);
}

void Networking::StartServer(uint16_t port, int numOfSockets)
{
	netEngine.StartServer(port, numOfSockets);
}

void Networking::Connect(std::string ip, uint16_t port)
{
	netEngine.Connect(ip, port);
}

int Networking::GetClientID()
{
	return netEngine.clientID;
}

bool Networking::IsConnected()
{
	if (netEngine.clientID == -1) { return false; }
	return true;
}