#include <iostream>
#include <string>
#include <bitset>
#include <vector>
#include <WS2tcpip.h>

// cd "c:\Users\Manjaro\Desktop\DT065A_Project\DT065A_Project\LABB2\MQTTClient\" ; if ($?) { g++ main.cpp -o main -lws2_32} ; if ($?) { .\main }
// cd "c:\Users\j_c_k\Desktop\DT065A_Project\DT065A_Project\LABB2\MQTTClient\" ; if ($?) { g++ main.cpp -o main -lws2_32} ; if ($?) { .\main }

std::vector<char> TextToBinChars(std::string str);

int main(){
    
    std::string ipAdress = "127.0.0.1";
    int port = 54010;

    // Initialize WinSock
    WSAData data;
    WORD ver = MAKEWORD(2, 2);
    int wsResult = WSAStartup(ver, &data);
    if(wsResult != 0){
        std::cerr << "Can't start Winsock, Err #" << wsResult << std::endl;
        return 0;
    }

    // Create socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET){
        std::cerr << "Can't create socket, Err #" << WSAGetLastError() << std::endl;
        return 0;
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
        return 0;
    }

    // Do-while loop to send and receive data
    char buf[65535];
    std::string userInput;

    do
    {
        // Prompt the user for some text
        std::cout << "> ";
        std::getline(std::cin, userInput);

        std::vector<char> sendReady = TextToBinChars(userInput);
        for(char c: sendReady){
            std::cout << c;
        }
        std::cout << std::endl;

        std::string receivedMsg;

        if(userInput.size() > 0){       // Make sure the user has typed in something
            // Send the text
            int sendResult = send(sock, sendReady.data(), sendReady.size(), 0);
            if(sendResult != SOCKET_ERROR){
                // Wait for response
                ZeroMemory(buf, 65535);
                int bytesReceived = recv(sock, buf, 65535, 0);
                if(bytesReceived > 0){
                    // Echo response to console
                    receivedMsg = buf;
                    std::cout << "SERVER>" << std::string(receivedMsg, 0, bytesReceived) << std::endl;
                }
            }
        }
    } while (userInput.size() > 0);

    // Gracefully close down everything
    closesocket(sock);
    WSACleanup();
}


std::vector<char> TextToBinChars(std::string str){
    std::string msgStr = "";

    std::vector<char> bodyChar(str.begin(), str.end());
    for(char c: bodyChar){
        msgStr += std::bitset<8>(c).to_string();
    }

    std::vector<char> sendReady;
    for(char c: msgStr){
        sendReady.push_back(c);
    }

    return sendReady;
}

// POST / HTTP/1.1/\r\nHeader Stuff\r\n\r\nSensor
// PUT /Sensor/ HTTP/1.1/\r\nHeader Stuff\r\n\r\n69
// GET /Sensor/ HTTP/1.1/\r\nHeader Stuff\r\n\r\nBody
// DELETE /Sensor/ HTTP/1.1/\r\nHeader Stuff\r\n\r\nBody
    