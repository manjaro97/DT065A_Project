#include "MQTTBroker_TcpListener.h"
#include "MQTTBroker_msgHandler.h"
#include "MQTTBroker_PublishHandler.h"
#include "MQTTBroker_Header.h"
#include "MQTTBroker_database.h"

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

	std::cout << ".MQTTserver. ---Server Started---" << std::endl;
	
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
		cout << ".MQTTserver. " << host << " connected on port " << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << ".MQTTserver. " << host << " connected on port " <<
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
			cout << ".MQTTserver. " << "Client disconnected " << endl;
			break;
		}
		if (bytesReceived == SOCKET_ERROR)
		{
			cerr << ".MQTTserver. " << "Error in recv(). Quitting" << endl;
			break;
		}

        std::string receivedMsgBuffer = "";
		for (int i = 0; i < bytesReceived; i++) {
			receivedMsgBuffer += std::bitset<8>(buf[i]).to_string();
		}
        std::vector<char> responseMsg = HandleRequest(receivedMsgBuffer, threadClientSocket);

        send(threadClientSocket, responseMsg.data(), responseMsg.size(), 0);

		//Send retained Msg
		std::pair<std::string, std::string> retainedMsg = databaseObj.GetRetainedMsg(threadClientSocket);
		if(retainedMsg.second != ""){
			std::cout << ".MQTTserver. Sending retained msg: " << std::endl;
			std::vector<char> subscriptionMsg = SendPublish(0, retainedMsg.first, retainedMsg.second);
			send(threadClientSocket, subscriptionMsg.data(), subscriptionMsg.size(), 0);
		}

		//Send out Data to all Subscribers
		std::map<std::string, std::vector<SOCKET>> subscriptions = databaseObj.GetListOfSubscriptions();
		for(pair<std::string, std::string> p: databaseObj.GetNewData()){
			std::vector<SOCKET> list = subscriptions[p.first];
			if(list.size() != 0){
				std::vector<char> subscriptionMsg = SendPublish(0, p.first, p.second);
				for(SOCKET socket: list){
					send(socket, subscriptionMsg.data(), subscriptionMsg.size(), 0);
				}
			}
		}
		databaseObj.EraseDataFromQueue();

	};
	// Close the socket
	closesocket(threadClientSocket);

	return 0;
}