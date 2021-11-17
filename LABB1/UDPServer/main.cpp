#include <iostream>
#include "convertCoAP.h"
#include <WS2tcpip.h>

//#pragma comment (lib, "ws2_32.lib")

// cd "C:\Users\j_c_k\Desktop\DT065A_Project\DT065A_Project\LABB1\UDPServer\" ; if ($?) { g++ main.cpp convertCoAP.h convertCoAP.cpp -o main -lws2_32} ; if ($?) { .\main }
// cd "C:\Users\Manjaro\Desktop\DT065A_Project\DT065A_Project\LABB1\UDPServer\" ; if ($?) { g++ main.cpp convertCoAP.h convertCoAP.cpp -o main -lws2_32} ; if ($?) { .\main }

int main(){
    

    // Initialize winsock
    WSADATA wsData;
    //Using winsock version 2.2
    WORD ver = MAKEWORD(2, 2);

    //Initialize winsock
    int wsOK = WSAStartup(ver, &wsData);
    //WSAStartup returns 0 if successful and other errorcodes if not
    if(wsOK != 0){
        std::cout  << "Error: " << wsOK << "Can't initialize winsock! Quitting!" << std::endl;
        return 0;
    }
    
    // Create a socket
    //Socket is bound by IPadress and Port

    //AF_INET means that we are using the IPv4 adress family
    //SOCK_STREAM means that we are using a TCP socket, UDP would be SOCK_DGRAM
    SOCKET listening = socket(AF_INET, SOCK_DGRAM, 0);
    if(listening == INVALID_SOCKET){
        std::cerr << "Can't create a socket! Quitting?" << std::endl;
        return 0;
    }

    // Bind the ip address and port to a socket
    
    //hint structure needs to be filled in
    sockaddr_in hint;
    //set address family
    hint.sin_family = AF_INET;
    //set port by host-to-network-short
    hint.sin_port = htons(5683);
    //bind to Any address
    hint.sin_addr.S_un.S_addr = ADDR_ANY; 

    //bind port and Any address to the socket
    if(bind(listening, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR){
        std::cout << "Can't bind socket! " << WSAGetLastError() << std::endl;
        return 0;
    }

    // Tell winsock the socket is for listening
    
    
    // Wait for a connection
    sockaddr_in client;
    int clientLength = sizeof(client);
    ZeroMemory(&client, clientLength);

    char buf[1024];

    while(true){
        ZeroMemory(buf, 1024);

        // Wait for client to send data
        int bytesReceived = recvfrom(listening, buf, 1024, 0, (sockaddr*)&client, &clientLength);
        if(bytesReceived == SOCKET_ERROR){
            std::cerr << "Error in recvfrom(). Quitting!" << std::endl;
            break;
        }
        
        // Display message and client info
        char clientIp[256];
        ZeroMemory(clientIp, 256);

        inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);

        std::cout << "Message recv from " << clientIp << " : " << buf << std::endl;
        
        std::vector<char> receivedMsg;
        for (char c: buf){
            receivedMsg.push_back(c);
        }
        
        std::vector<std::string> translatedMsg = fromCoAP(receivedMsg);
        for(std::string s1 : translatedMsg){
            std::cout << "Received Message: " << s1 << std::endl;
        }
    }

    // Close listening socket
    closesocket(listening);

    // Cleanup winsock
    WSACleanup();
    
}