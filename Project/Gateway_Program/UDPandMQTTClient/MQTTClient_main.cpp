#include <iostream>
#include <string>
#include <bitset>
#include <vector>
#include <WS2tcpip.h>
#include "MQTTClient_ConnectHandler.h"
#include "MQTTClient_pingHandler.h"
#include "MQTTClient_PublishHandler.h"
#include <windows.h>
#include "dataHeader.h"


// cd "c:\Users\j_c_k\Desktop\DT065A_Project\DT065A_Project\Project\Gateway_Program\MQTTClient\" ; if ($?) { g++ *.cpp -o main -lws2_32} ; if ($?) { .\main }

void runMQTTClient(){
    
    std::string ipAdress = "127.0.0.1";
    int port = 1883;

    // Initialize WinSock
    WSAData data;
    WORD ver = MAKEWORD(2, 2);
    int wsResult = WSAStartup(ver, &data);
    if(wsResult != 0){
        std::cerr << "Can't start Winsock, Err #" << wsResult << std::endl;
        return;
    }

    // Create socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET){
        std::cerr << "Can't create socket, Err #" << WSAGetLastError() << std::endl;
        return;
    }

    // Fill in a hint structure
    sockaddr_in hint; 
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAdress.c_str(), &hint.sin_addr);

    // Connect to server
    int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if(connResult == SOCKET_ERROR){
        std::cerr << "Can't connect to server, Err #" << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return;
    }

    // Do-while loop to send and receive data
    char buf[65535];
    std::string userInput;

    std::vector<char> sendReady = SendConnect();
    int sendResult = send(sock, sendReady.data(), sendReady.size(), 0);
    ZeroMemory(buf, 65535);
    int bytesReceived = recv(sock, buf, 65535, 0);
    if(bytesReceived > 0){
        // Echo response to console
        std::cout << ".MQTTclient. Connected to Server" << std::endl;
    }                 

    do
    {
        //Wait for 10 Seconds
        std::cout << ".MQTTclient. ---Waiting 10 seconds---" << std::endl;
        Sleep(10000);

        std::cout << ".MQTTclient. Sending Publish to Broker" << std::endl;
        std::map<std::string, std::string> m = dataObj.GetData();
        for(std::pair<std::string, std::string> p : m){
           
        std::vector<char> sendReady = clientSendPublish(0, p.first, p.second);
        
        // Send the text
        int sendResult = send(sock, sendReady.data(), sendReady.size(), 0);

        }
        
    } while (true);

    // Gracefully close down everything
    closesocket(sock);
    WSACleanup();

    return;
}