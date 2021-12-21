#include <iostream>
#include <string>
#include <bitset>
#include <sstream>
#include "sendMessage.h"
#include "convertCoAP.h"
#include<windows.h>

// cd "C:\Users\Manjaro\Desktop\DT065A_Project\DT065A_Project\Project\Gateway_Program\UDPClient\" ; if ($?) { g++ main.cpp sendMessage.cpp sendMessage.h convertCoAP.cpp convertCoAP.h -o main -lws2_32} ; if ($?) { .\main }
// cd "C:\Users\j_c_k\Desktop\DT065A_Project\DT065A_Project\Project\Gateway_Program\UDPClient\" ; if ($?) { g++ main.cpp sendMessage.cpp sendMessage.h convertCoAP.cpp convertCoAP.h -o main -lws2_32} ; if ($?) { .\main }

int main(){

    //Test 
    std::vector<std::string> tester;
        
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
    
    std::string OptionValue = "sensor";
    //OptionValue is [5]
    tester.push_back(OptionValue); // Options
    
    std::string Payload = "test";
    //Payload is [6]
    tester.push_back(Payload); // Payload
    //----------------------------------

    while(true){

        std::cout << "Ready..." << std::endl;
        
        try {
            std::cout << "Sending " << code << " request for sensor: " << OptionValue << std::endl;
            std::vector<char> returnMsg = sendMessage(toCoAP(tester));

            std::cout << "Reading Response" << std::endl;
            std::vector<std::string> s = fromCoAP(returnMsg);
            /*for (std::string str : s){
                std::cout << str << std::endl;
            }*/
            std::cout << "Received from Sensor: " << s[5] << " the value: " << s[7] << std::endl;
        }
        catch (...) {
            std::cout << "Failed to Send Message" << std::endl;
        }

        std::cout << "\nWaiting 60 seconds...\n" << std::endl;
        Sleep(10000);

    }

}