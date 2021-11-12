#include <iostream>
#include <string>
#include "sendMessage.h"
#include "convertCoAP.h"

// cd "C:\Users\j_c_k\Desktop\DT065A_Project\DT065A_Project\LABB1\UDPClient\" ; if ($?) { g++ main.cpp sendMessage.cpp sendMessage.h -o main -lws2_32} ; if ($?) { .\main }

int main(){

    std::string msg;

    while(true){
        std::cout << "Write a message you want to send: ";
        std::getline(std::cin, msg);

        //Test Message
        std::vector<std::string> tester;
        std::string testMsg = "Hello World";
        std::string ver = "1";
        //Ver is [0]
        tester.push_back(ver); // CoAP version: (01) is version 1
        std::string T = "NON";
        //T is [1]
        tester.push_back(T); // Message type: CON (00), NON (01), ACK (10) and RST (11)
        std::string code = "GET";
        //code is [2]
        tester.push_back(code); // Method: Empty (0000 0000), GET (0000 0001), POST (0000 0010), PUT (0000 0011), DELETE (0000 0100)
        std::string Token = "";
        //Token is [3]
        tester.push_back(Token); // Token
        std::string OptionDelta = "Uri-Path";
        //OptionDelta is [4]
        tester.push_back(OptionDelta); // Options
        std::string OptionValue = "/path/";
        //OptionValue is [5]
        tester.push_back(OptionValue); // Options
        std::string Payload = "";
        //Payload is [6]
        tester.push_back(Payload); // Payload
        //----------------------------------



        std::vector<char> msgCoAP = toCoAP(tester);

        std::cout << sendMessage(msg) << std::endl;
    }

}