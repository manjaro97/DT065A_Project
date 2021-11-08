#include <iostream>
#include <string>
#include <vector>

#include "TCPlistener.h"

// cd "c:\Users\j_c_k\Desktop\DT065A_Project\DT065A_Project\TCPListener\" ; if ($?) { g++ main.cpp TcpListener.cpp TcpListener.h -o main -lws2_32 } ; if ($?) { .\main }

void Listener_MessageReceived(CTcpListener* listener, int client, std::string msg);
std::vector<std::string> SplitHeaderBody(std::string given_str, std::string delimiter);

int main(){

    CTcpListener server("127.0.0.1", 54010, Listener_MessageReceived);
    
    if(server.init()){
        server.Run();
    }
}

void Listener_MessageReceived(CTcpListener* listener, int client, std::string msg){

    std::string given_str = msg; // GET /Address/ HTTP/1.1/\r\nHeader Stuff\r\nDoc-TYPE = lol\r\n\r\nBody - Testing hoping this shit works sometime
    
    std::vector<std::string> msgSplit = SplitHeaderBody(given_str, "\\r\\n\\r\\n");
    if(msgSplit.size() == 2){
        std::cout << "Header: "<< msgSplit[0] << std::endl;
        std::cout << "Body: " << msgSplit[1] << std::endl;

        std::vector<std::string> headerSplit = SplitHeaderBody(msgSplit[0], "\\r\\n");

        if(headerSplit.size() != 0){
            std::vector<std::string> requestSplit = SplitHeaderBody(headerSplit[0], " ");
            
            if(requestSplit.size() == 3){
                // Not needed
                std::string request = requestSplit[0];
                std::string address = requestSplit[1];
                std::string requestType = requestSplit[2];

                // Not needed
                std::cout << "request: " << request << std::endl;
                std::cout << "address: " << address << std::endl;
                std::cout << "requestType: " << requestType << std::endl;
            }
            else{
                listener->Send(client, "400 - Error: Request Splitting Failed, Faulty request");
                return;
            }
        }
        else{
            listener->Send(client, "400 - Error: Header Splitting Failed, Faulty request");
            return;
        }
    }
    else{
        listener->Send(client, "400 - Error: Message Splitting Failed, Faulty request");
        return;
    }    

    listener->Send(client, "HTTP Status 200 (OK)");
    std::cout << "Message: " << "HTTP Status 200 (OK)" << std::endl;
    return;
}

std::vector<std::string> SplitHeaderBody(std::string given_str, std::string delimiter){
    
    std::vector<std::string> msgSplit; 
    size_t pos = 0;

    if(given_str.find (delimiter) == std::string::npos){
        return msgSplit;
    }

    while (( pos = given_str.find (delimiter)) != std::string::npos)  
    {  
        msgSplit.push_back(given_str.substr(0, pos)); // store the substring  
        given_str.erase(0, pos + delimiter.length()); // erase previous substring   
    }  
    msgSplit.push_back(given_str);

    return msgSplit;
}