#include <iostream>
#include <string>
#include <bitset>
#include <sstream>
#include "sendMessage.h"
#include "convertCoAP.h"

// cd "C:\Users\Manjaro\Desktop\DT065A_Project\DT065A_Project\Project\Gateway_Program\UDPClient\" ; if ($?) { g++ main.cpp sendMessage.cpp sendMessage.h convertCoAP.cpp convertCoAP.h -o main -lws2_32} ; if ($?) { .\main }
// cd "C:\Users\j_c_k\Desktop\DT065A_Project\DT065A_Project\Project\Gateway_Program\UDPClient\" ; if ($?) { g++ main.cpp sendMessage.cpp sendMessage.h convertCoAP.cpp convertCoAP.h -o main -lws2_32} ; if ($?) { .\main }

int main(){

    std::cout << "Hello: ";
    while(true){
        //Test 
        std::vector<std::string> tester;
        
        std::string ver = "1";
        //std::cout << "CoAP Version (standard 1): ";
        //std::getline(std::cin, ver);
        //Ver is [0]
        tester.push_back(ver); // CoAP version: (01) is version 1

        std::string T = "NON";
        //std::cout << "Message Type (standard NON): ";
        //std::getline(std::cin, T);
        //T is [1]
        tester.push_back(T); // Message type: CON (00), NON (01), ACK (10) and RST (11)
        
        std::string code = "GET";
        std::cout << "Request (GET): ";
        std::getline(std::cin, code);
        //code is [2]
        tester.push_back(code); // Method: Empty (0000 0000), GET (0000 0001), POST (0000 0010), PUT (0000 0011), DELETE (0000 0100)
        
        std::string Token = "";
        //std::cout << "Token (standard null): ";
        //std::getline(std::cin, Token);
        //Token is [3]
        tester.push_back(Token); // Token
        
        std::string OptionDelta = "Uri-Path";
        //std::cout << "Option delta (standard Uri-Path): ";
        //std::getline(std::cin, OptionDelta);
        //OptionDelta is [4]
        tester.push_back(OptionDelta); // Options
        
        std::string OptionValue = "/path/";
        std::cout << "Option Value (standard /path/): ";
        std::getline(std::cin, OptionValue);
        //OptionValue is [5]
        tester.push_back(OptionValue); // Options
        
        std::string Payload = "";
        std::cout << "Body: ";
        std::getline(std::cin, Payload);
        //Payload is [6]
        tester.push_back(Payload); // Payload
        //----------------------------------
        std::cout << "\n\n";
        
        std::vector<char> returnMsg = sendMessage(toCoAP(tester));
        std::vector<std::string> s = fromCoAP(returnMsg);
        for (std::string str : s){
            std::cout << str << std::endl;
        }

    }

}