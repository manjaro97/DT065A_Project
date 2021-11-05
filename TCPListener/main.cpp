#include <iostream>
#include <string>

#include "TCPlistener.h"

// cd "c:\Users\j_c_k\Desktop\DT065A_Project\DT065A_Project\TCPListener\" ; if ($?) { g++ main.cpp TcpListener.cpp TcpListener.h -o main -lws2_32 } ; if ($?) { .\main }

void Listener_MessageReceived(CTcpListener* listener, int client, std::string msg);

int main(){

    CTcpListener server("127.0.0.1", 54010, Listener_MessageReceived);
    
    if(server.init()){
        server.Run();
    }
}

void Listener_MessageReceived(CTcpListener* listener, int client, std::string msg){
    listener->Send(client, msg);
    std::cout << "Message: " << msg << std::endl;
}