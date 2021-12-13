#include "TcpListener.h"
#include "msgHandler.h"
//MqttDB mqttDB; // Start database

TcpConnection::~TcpConnection()
{
	// Close listening socket
	closesocket(listening);

	// Cleanup winsock
	WSACleanup();
}

void TcpConnection::Start() {
	// Initialze winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		cerr << "Can't Initialize winsock! Quitting" << endl;
		return;
	}

    //TODO: TESTING
    std::cout << "TESTING: 1" << std::endl;
    //TODO: 

	// Create a socket
	listening = socket(AF_INET, SOCK_STREAM, 0); // SOCK_STREAM (TCP protocol)
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create a socket! Quitting" << endl;
		return;
	}

	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton .... 

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// Tell Winsock the socket is for listening 
	listen(listening, SOMAXCONN);

	SOCKET clientSocket;
	sockaddr_in client;
	// Wait for a connection
	int clientSize = sizeof(client);

	while (clientSocket = accept(listening, (sockaddr*)&client, &clientSize)) {
		if (clientSocket == INVALID_SOCKET)
		{
			cerr << "Invalid client socket" << endl;
			continue;
		}
		_beginthreadex(0, 0, ServClient, (void*)&clientSocket, 0, 0);
	}
}

unsigned int __stdcall ServClient(void* data)
{
	sockaddr_in client;
	SOCKET* clientSocket = (SOCKET*)data;
	SOCKET threadClientSocket = *clientSocket;

	char host[NI_MAXHOST];		// Client's remote name
	char service[NI_MAXSERV];	// Service (i.e. port) the client is connect on

	ZeroMemory(host, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << host << " connected on port " << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " <<
			ntohs(client.sin_port) << endl;
	}

	char buf[65535];
	// While loop: accept and echo message back to client
	while (true)
	{
		ZeroMemory(buf, 65535);

		// Wait for client to send data
		int bytesReceived = recv(threadClientSocket, buf, 65535, 0);

		if (bytesReceived == 0)
		{
			cout << "Client disconnected " << endl;
			break;
		}
		if (bytesReceived == SOCKET_ERROR)
		{
			cerr << "Error in recv(). Quitting" << endl;
			break;
		}

        std::string receivedMsgBuffer = "";
		for (int i = 0; i < bytesReceived; i++) {
			receivedMsgBuffer += std::bitset<8>(buf[i]).to_string();
		}
        std::vector<char> responseMsg = HandleRequest(receivedMsgBuffer.substr(0, 4), receivedMsgBuffer);

        send(threadClientSocket, responseMsg.data(), responseMsg.size(), 0);

        /*
		if (!handleMsg.GetSendSingleResponseFlag())
		{
			send(threadClientSocket, handleMsg.GetResponse().data(), handleMsg.GetResponse().size(), 0);
			if (handleMsg.GetRetaindFlag())
			{
				send(threadClientSocket, handleMsg.GetRetainedResponse().data(), handleMsg.GetRetainedResponse().size(), 0);
			}
		}
		else
		{
			vector<SOCKET> subs = handleMsg.GetMultipleResponses();
			if (!subs.empty()) {
				for (auto subscriberSocket : subs) {
					send(subscriberSocket, handleMsg.GetResponse().data(), handleMsg.GetResponse().size(), 0);
				}
			}
		}*/

	};
	// Close the socket
	closesocket(threadClientSocket);

	return 0;
}