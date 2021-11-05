#ifndef TCPLISTENER_H
#define TCPLISTENER_H

#include <string>
#include <WS2tcpip.h>

class CTcpListener;

typedef void (*MessageReceivedHandler)(CTcpListener* listener, int socketId, std::string msg);

class CTcpListener{
    public:

    CTcpListener(std::string ipAddress, int port, MessageReceivedHandler handler);

    ~CTcpListener();

    // Send message back to client
    void Send(int clientSocket, std::string msg);

    // Initialize winsock   
    bool init();

    // Loop
    void Run();
    
    // Cleanup winsock
    void Cleanup();

    private:

    // Create a socket
    SOCKET CreateSocket();

    // Wait for a connection
    SOCKET WaitForConnection(SOCKET listening);

    std::string             m_ipAddress;
    int                     m_port;
    MessageReceivedHandler  MessageReceived;

};

#endif