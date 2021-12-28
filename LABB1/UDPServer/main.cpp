#include <iostream>
#include "convertCoAP.h"
#include <WS2tcpip.h>
#include <fstream>
#include <string>
#include <bitset>
#include <map>
#include <process.h>
#include <random>

//#pragma comment (lib, "ws2_32.lib")

// cd "C:\Users\Manjaro\Desktop\DT065A_Project\DT065A_Project\Project\Sensor_Program\" ; if ($?) { g++ main.cpp convertCoAP.h convertCoAP.cpp -o main -lws2_32} ; if ($?) { .\main }
// cd "C:\Users\j_c_k\Desktop\DT065A_Project\DT065A_Project\Labb1\UDPServer\" ; if ($?) { g++ main.cpp convertCoAP.h convertCoAP.cpp -o main -lws2_32} ; if ($?) { .\main }

unsigned int __stdcall ServClient(void* data);
std::vector<std::string> prepareMsg(std::string path);
std::map<std::string, std::string> GetSensorData();
void PushToDatabase(std::map<std::string, std::string> m);

int main(){

    _beginthreadex(0, 0, ServClient, 0, 0, 0);

    std::map<std::string, std::string> pathAndData = GetSensorData();
    std::string path = "sensor";

    //Add data of Topic to database
    if(pathAndData.size() == 0 || pathAndData.find(path) == pathAndData.end()){
        std::pair<std::string, std::string> sensor(path, "-waiting for data-");
        pathAndData.insert(sensor);
        PushToDatabase(pathAndData);
    }

    while(true){
        std::cout << "***Getting New Sensor Data***" << std::endl;

        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist6(1,128);

        std::string s = std::to_string(dist6(rng));

        std::cout << "New data: " << s << std::endl;

        pathAndData[path] = s;

        PushToDatabase(pathAndData);

        Sleep(60000);//60 seconds
    }
}

unsigned int __stdcall ServClient(void* data){
    

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
    int port = 53000;
    hint.sin_port = htons(port);
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

    char buf[65535];

    std::cout << "Starting Server on Port: " << port << std::endl;

    while(true){
        ZeroMemory(buf, 65535);

        // Wait for client to send data
        int bytesReceived = recvfrom(listening, buf, 65535, 0, (sockaddr*)&client, &clientLength);
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

        for (int i = 0; i < bytesReceived; i++) {
			receivedMsg.push_back(buf[i]);
		}
        
        std::vector<std::string> translatedMsg = fromCoAP(receivedMsg);
        /*for(std::string s1 : translatedMsg){
            std::cout << "Received Message: " << s1 << std::endl;
        }*/
        std::string path = translatedMsg[5];
        std::cout << "Received " << translatedMsg[2] << " Request for sensor: " << path << std::endl;
        
        std::vector<char> msg = toCoAP(prepareMsg(path));

        // Write out to that socket
        std::cout << "Sending Response" << std::endl;
        int sendOk = sendto(listening, msg.data(), msg.size(), 0, (sockaddr*)&client, sizeof(client));

        if(sendOk == SOCKET_ERROR){
            std::cerr << "Message was not sent " << WSAGetLastError() << std::endl;
        }
    }

    // Close listening socket
    closesocket(listening);

    // Cleanup winsock
    WSACleanup();
    
    return 0;
}

std::vector<std::string> prepareMsg(std::string path){

    std::vector<std::string> responseMsg;
    responseMsg.push_back("1");
    responseMsg.push_back("NON");
    responseMsg.push_back("POST");
    responseMsg.push_back("");
    responseMsg.push_back("Uri-Path");
    responseMsg.push_back(path);
    
    std::map<std::string, std::string> m = GetSensorData();
    responseMsg.push_back(m[path]);

    return responseMsg;
    
}

std::map<std::string, std::string> GetSensorData(){
    std::map<std::string, std::string> m;
    
    std::ifstream database("database.txt");
    std::string s;
    std::string delimiter = ":";
    while(getline (database, s)) {
        std::string pathFromDB = s.substr(0, s.find(delimiter)); 
        s.erase(0, s.find(delimiter)+delimiter.length()); 
        std::string dataFromDB = s;
        
        std::pair<std::string, std::string> p(pathFromDB, dataFromDB);
        m.insert(p);
    }
    database.close();
    return m;
}

void PushToDatabase(std::map<std::string, std::string> m){
    std::ofstream database;
    
    database.open("database.txt", std::ofstream::out | std::ofstream::trunc);

    for(std::pair<std::string, std::string> p: m){
        database << p.first << ":" << p.second << std::endl;
    }

    database.close();

    return;
}