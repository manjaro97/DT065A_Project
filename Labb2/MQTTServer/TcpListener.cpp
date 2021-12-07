#include <iostream>
#include "TCPlistener.h"
#include <bitset>

    CTcpListener::CTcpListener(std::string ipAddress, int port, MessageReceivedHandler handler)
        : m_ipAddress(ipAddress),  m_port(port), MessageReceived(handler)
    {
        
    }

    CTcpListener::~CTcpListener(){
        Cleanup();
    }

    // Send message back to client
    void CTcpListener::Send(int clientSocket, std::vector<char> msg){
        send(clientSocket, msg.data(), msg.size(), 0);
    }

    // Initialize winsock   
    bool CTcpListener::init(){
        WSADATA data;
        WORD ver = MAKEWORD(2, 2);

        int wsInit = WSAStartup(ver, &data);
        //TODO: Error Output

        return wsInit == 0;
    }

    // Loop
    void CTcpListener::Run(){
        char buf[65535];

        while(true){

            // Create a listening socket
            SOCKET listening = CreateSocket();
            if(listening == INVALID_SOCKET){
                std::cerr << "Can't create a socket! Quitting?" << std::endl;
                break;
            }

            SOCKET client = WaitForConnection(listening);
            if(client != INVALID_SOCKET){
                closesocket(listening);

                int bytesReceived = 0;
                std::string receivedMsg = "";
                do{
                    ZeroMemory(buf, 65535);
                    bytesReceived = recv(client, buf, 65535, 0);
                    if(bytesReceived == SOCKET_ERROR){
                        std::cerr << "Error in recv(). Quitting!" << std::endl;
                    }
                    else if(bytesReceived > 0){
                        if(MessageReceived != NULL){
                            
                            receivedMsg = "";
                            for(int i = 0; i < bytesReceived; i++){
                                receivedMsg += std::bitset<8>(buf[i]).to_string();
                            } 
                            std::cout << "Received Message: " << receivedMsg << std::endl;
                            MessageReceived(this, client, std::string(receivedMsg, 0, bytesReceived*8));
                        }
                    }
                    else{
                        std::cerr << "User disconnected. Quitting!" << std::endl;
                    }
                }   while(bytesReceived > 0);

                closesocket(client);
            }
            else{
                std::cerr << "Client socket is invalid!" << std::endl;
            }

        }
    }
    
    // Cleanup winsock
    void CTcpListener::Cleanup(){
        WSACleanup();
    }

    // Create a socket
    SOCKET CTcpListener::CreateSocket(){
        SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
        if(listening != INVALID_SOCKET){
            sockaddr_in hint;
            hint.sin_family = AF_INET;
            hint.sin_port = htons(m_port);
            inet_pton(AF_INET, m_ipAddress.c_str(), &hint.sin_addr);

            int bindOk = bind(listening, (sockaddr*)&hint, sizeof(hint));
            if(bindOk != SOCKET_ERROR){
                int listenOk = listen(listening, SOMAXCONN);
                if(listenOk == SOCKET_ERROR){
                    return -1;
                }
            }
            else{
                return -1;
            }
        }
        return listening;
    }

    // Wait for a connection
    SOCKET CTcpListener::WaitForConnection(SOCKET listening){
        SOCKET client = accept(listening, NULL, NULL);
        return client;
    }