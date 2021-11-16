#include <iostream>
#include <WS2tcpip.h>
#include <string>

//#pragma comment (lib, "ws2_32.lib")

std::string sendMessage(char msg[]){  // We can pass in a command lline option!!
    

    // Initialize winsock
    WSADATA wsData;
    //Using winsock version 2.2
    WORD ver = MAKEWORD(2, 2);

    //Initialize winsock
    int wsOK = WSAStartup(ver, &wsData);
    //WSAStartup returns 0 if successful and other errorcodes if not
    if(wsOK != 0){
        std::cout  << "Error: " << wsOK << "Can't initialize winsock! Quitting!" << std::endl;
        return "Error";
    }

    // Create a hint structure for the server
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(5683);

    inet_pton(AF_INET, "134.102.218.18", &server.sin_addr);

    // Socket creation
    SOCKET out = socket(AF_INET, SOCK_DGRAM,0);

    // Write out to that socket
    int sendOk = sendto(out, msg, 1024, 0, (sockaddr*)&server, sizeof(server));

    if(sendOk == SOCKET_ERROR){
        std::cerr << "Message was not sent " << WSAGetLastError() << std::endl;
        return "Error";
    }

    int receive = recv

    // Close the socket
    closesocket(out);

    // Close down Winsock
    WSACleanup();

    return "Success";
}