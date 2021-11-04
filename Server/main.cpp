#include <iostream>
#include <WS2tcpip.h>

//#pragma comment (lib, "ws2_32.lib")

//cd "c:\Users\Manjaro\Desktop\DT065A_Project\DT065A_Project\" ; if ($?) { g++ main.cpp -o main -lws2_32 } ; if ($?) { .\main }

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
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
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
    hint.sin_port = htons(54000);
    //bind to Any address
    hint.sin_addr.S_un.S_addr = INADDR_ANY; //could also use inet_pton

    //bind port and Any address to the socket
    bind(listening, (sockaddr*)&hint, sizeof(hint));

    // Tell winsock the socket is for listening
    
    //Make ready for listening
    //SOMAXCONN sets max amount of connections
    listen(listening, SOMAXCONN);

    // Wait for a connection
    sockaddr_in client;
    int clientSize = sizeof(client);

    SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
    
    if(clientSocket == INVALID_SOCKET){
        std::cerr << "Client socket is invalid!" << std::endl;
    }
    
    char host[NI_MAXHOST];      //Client's remote name
    char service[NI_MAXSERV];   //Service (i.e. port) the client is connected on

    ZeroMemory(host, NI_MAXHOST);
    ZeroMemory(service, NI_MAXSERV);
    
    if(getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0){
        std::cout << host << " connected on port " << service << std::endl;
    }
    else{
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << host << " connected on port " << ntohs(client.sin_port) << std::endl;
    }
    

    // Close listening socket
    closesocket(listening);

    // While loop: accept and echo back to client
    char buf[4096];

    std::cout << " Starting while loop " << std::endl;

    while(true){
        ZeroMemory(buf, 4096);

        // Wait for client to send data
        int bytesReceived = recv(clientSocket, buf, 4096, 0);
        if(bytesReceived == SOCKET_ERROR){
            std::cerr << "Error in recv(). Quitting!" << std::endl;
            break;
        }
        if(bytesReceived == 0){
            std::cerr << "User disconnected. Quitting!" << std::endl;
            break;
        }
        if(sizeof(buf) > 0){
            std::cout << "Messages: " << buf << std::endl;
        }

        // Echo message back to client
        send(clientSocket, buf, bytesReceived +1, 0);
    }
    std::cout << " Ending while loop " << std::endl;

    // Close the socket
    closesocket(clientSocket);

    // Cleanup winsock
    WSACleanup();
    
}