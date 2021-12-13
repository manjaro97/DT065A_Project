#ifndef TCPLISTENER_H
#define TCPLISTENER_H

#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <queue>
#include <bitset>
#include <process.h>
#include "msgHandler.h"

using namespace std;

unsigned int __stdcall ServClient(void* data);

class TcpConnection
{
	SOCKET listening;	// Create a socket
	int port = 1883; // MQTT port

public:
	~TcpConnection();
	void Start();
};

#endif