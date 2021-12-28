#include <iostream>
#include <string>
#include <bitset>
#include <vector>
#include <WS2tcpip.h>

// cd "c:\Users\j_c_k\Desktop\DT065A_Project\DT065A_Project\Project\Gateway_Program\MQTTClient\" ; if ($?) { g++ *.cpp -o main -lws2_32} ; if ($?) { .\main }

std::vector<char> SendConnect();

int main(){
    
    std::string ipAdress = "127.0.0.1";
    int port = 1883;

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

    std::vector<char> sendReady = SendConnect();
    int sendResult = send(sock, sendReady.data(), sendReady.size(), 0);
    ZeroMemory(buf, 65535);
    int bytesReceived = recv(sock, buf, 65535, 0);
    if(bytesReceived > 0){
        // Echo response to console
        std::cout << "SERVER>" << std::string(buf, 0, bytesReceived) << std::endl;
    }

    do
    {
        // Prompt the user for some text
        std::cout << "> ";
        std::getline(std::cin, userInput);

        //TODO: Change SendConnect to SendData
        std::vector<char> sendReady = SendConnect();

        std::string receivedMsg;

        //TODO: Should probs remove the Userinput
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
    
std::vector<char> SendConnect(){

    std::vector<std::string> s1;
        
    s1.push_back("00010000");//Header
    s1.push_back("00001010");//Length of rest
    s1.push_back("00000000");//Length MSB
    s1.push_back("00000100");//Length LSB
    s1.push_back("01001101");//M
    s1.push_back("01010001");//Q
    s1.push_back("01010100");//T
    s1.push_back("01010100");//T
    s1.push_back("00000101");//Version 5
    s1.push_back("00000000");//Connect Flags
    s1.push_back("00000000");//Keep Alive MSB
    s1.push_back("00000000");//Keep Alive LSB

    std::vector<char> sendReady; 
    for(std::string s: s1){
        for(char c: s){
            sendReady.push_back(c);
        }
    }

    return sendReady;

}