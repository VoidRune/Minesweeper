#pragma once
#include "Network/NetworkEngine.h"
#include "Entity/Entity.h"

class Networking
{
public:

	static inline Networking& Get() { return *s_Instance; }
	Networking() { s_Instance = this; };
	~Networking() {};

	void StartServer(uint16_t port, int numOfSockets = 8);
	void Connect(std::string ip, uint16_t port);

	void ProcessMessages();
	void SendMessageToAll(message& msg);
	void SendMessageExcept(message& msg, int excludedID);

	int GetClientID();
	bool IsConnected();

private:

	NetworkEngine netEngine;
	Entity entity;

	static Networking* s_Instance;
};

