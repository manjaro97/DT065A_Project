#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <bitset>

#include "TCPlistener.h"
#include "msgHandler.h"

// cd "c:\Users\Manjaro\Desktop\DT065A_Project\DT065A_Project\LABB2\MQTTServer\" ; if ($?) { g++ main.cpp TcpListener.cpp TcpListener.h msgHandler.h msgHandler.cpp ConnectHandler.h ConnectHandler.cpp PingHandler.h  PingHandler.cpp PublishHandler.h PublishHandler.cpp SubscribeHandler.h  SubscribeHandler.cpp -o main -lws2_32 } ; if ($?) { .\main }
// cd "c:\Users\j_c_k\Desktop\DT065A_Project\DT065A_Project\LABB2\MQTTServer\" ; if ($?) { g++ main.cpp TcpListener.cpp TcpListener.h msgHandler.h msgHandler.cpp ConnectHandler.h ConnectHandler.cpp PingHandler.h  PingHandler.cpp PublishHandler.h PublishHandler.cpp SubscribeHandler.h  SubscribeHandler.cpp -o main -lws2_32 } ; if ($?) { .\main }

// main.cpp TcpListener.cpp TcpListener.h msgHandler.h msgHandler.cpp ConnectHandler.h ConnectHandler.cpp PingHandler.h  PingHandler.cpp PublishHandler.h PublishHandler.cpp SubscribeHandler.h  SubscribeHandler.cpp

void Listener_MessageReceived(CTcpListener* listener, int client, std::string msg);

int main(){

    CTcpListener server("127.0.0.1", 1883, Listener_MessageReceived);
    
    if(server.init()){
        server.Run();
    }
}

void Listener_MessageReceived(CTcpListener* listener, int client, std::string msg){

    std::cout << "LOOK HERE: " << msg << std::endl;

    std::vector<char> responseMsg = HandleRequest(msg.substr(0, 4), msg);
    
    listener->Send(client, responseMsg);
    for(char c: responseMsg){
        std::cout << c;
    }
    std::cout << std::endl;

    return;
}