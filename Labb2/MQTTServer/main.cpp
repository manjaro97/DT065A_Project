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

std::vector<char> TextToBinChars(std::string str);
void Listener_MessageReceived(CTcpListener* listener, int client, std::string msg);

int main(){

    CTcpListener server("127.0.0.1", 54010, Listener_MessageReceived);
    
    if(server.init()){
        server.Run();
    }
}

void Listener_MessageReceived(CTcpListener* listener, int client, std::string msg){

    std::string given_str = msg; // GET /Address/ HTTP/1.1/\r\nHeader Stuff\r\nDoc-TYPE = lol\r\n\r\nBody - Testing hoping this shit works sometime
    std::string msgRequest = "*Basic* HTTP Status 200 (OK)";
    std::cout << "LOOK HERE: " << msg << std::endl;

    std::vector<std::string> msgSplit = SplitHeader(given_str);

    for(std::string s: msgSplit)   {
        std::cout << s << std::endl;
    }

    msgRequest = msgSplit[0];
    std::string temp = HandleRequest(msgRequest, given_str);
    
    // TODO: Remove this
    std::vector<char> sendReady = TextToBinChars(msgRequest);
    listener->Send(client, sendReady);
    std::cout << "Message: " << msgRequest << std::endl;
    for(char c: sendReady){
        std::cout << c;
    }
    std::cout << std::endl;

    return;
}

// TODO: Remove this
std::vector<char> TextToBinChars(std::string str){
    std::string msgStr = "";

    std::vector<char> bodyChar(str.begin(), str.end());
    for(char c: bodyChar){
        msgStr += std::bitset<8>(c).to_string();
    }

    std::vector<char> sendReady;
    for(char c: msgStr){
        sendReady.push_back(c);
    }

    return sendReady;
}