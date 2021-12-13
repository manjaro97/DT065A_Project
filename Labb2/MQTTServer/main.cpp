#include <iostream>
#include <string>
#include <vector>

#include "TCPlistener.h"
#include "msgHandler.h"

// cd "c:\Users\Manjaro\Desktop\DT065A_Project\DT065A_Project\LABB2\MQTTServer\" ; if ($?) { g++ main.cpp TcpListener.cpp TcpListener.h msgHandler.h msgHandler.cpp ConnectHandler.h ConnectHandler.cpp PingHandler.h  PingHandler.cpp PublishHandler.h PublishHandler.cpp SubscribeHandler.h  SubscribeHandler.cpp -o main -lws2_32 } ; if ($?) { .\main }
// cd "c:\Users\j_c_k\Desktop\DT065A_Project\DT065A_Project\LABB2\MQTTServer\" ; if ($?) { g++ main.cpp TcpListener.cpp TcpListener.h msgHandler.h msgHandler.cpp ConnectHandler.h ConnectHandler.cpp PingHandler.h  PingHandler.cpp PublishHandler.h PublishHandler.cpp SubscribeHandler.h  SubscribeHandler.cpp -o main -lws2_32 } ; if ($?) { .\main }

int main(){

    TcpConnection connection;
	connection.Start();

	return 0;
}