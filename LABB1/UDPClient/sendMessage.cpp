#include <iostream>
#include <WS2tcpip.h>
#include <string>

//#pragma comment (lib, "ws2_32.lib")

std::string sendMessage(std::string msg){  // We can pass in a command lline option!!
    

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
    server.sin_port = htons(54000);

    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    // Socket creation
    SOCKET out = socket(AF_INET, SOCK_DGRAM,0);

    // Write out to that socket
    int sendOk = sendto(out, msg.c_str(), msg.size() + 1, 0, (sockaddr*)&server, sizeof(server));

    if(sendOk == SOCKET_ERROR){
        std::cerr << "Message was not sent " << WSAGetLastError() << std::endl;
        return "Error";
    }

    // Close the socket
    closesocket(out);

    // Close down Winsock
    WSACleanup();

    return "Success";
}