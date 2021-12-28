#include <iostream>
#include <string>
#include <bitset>
#include <sstream>
#include "UDPClient_sendMessage.h"
#include "UDPClient_convertCoAP.h"
#include <windows.h>
#include "dataHeader.h"

DATA dataObj;
std::vector<std::string> createGetMessage(std::string OptionValue);

void runUDPClient(){

    std::string tempSensor = "temperature";
    std::string humSensor = "humidity";

    std::vector<std::string> temp = createGetMessage(tempSensor);
    std::vector<std::string> hum = createGetMessage(humSensor);
    
    while(true){

        try {
            std::cout << ".UDPclient.  Sending GET request for sensor: " << tempSensor << std::endl;
            std::vector<char> returnMsg = sendMessage(toCoAP(temp));

            std::vector<std::string> s = fromCoAP(returnMsg);
            /*for (std::string str : s){
                std::cout << str << std::endl;
            }*/
            std::cout << ".UDPclient.  Received from " << tempSensor << ": " << s[5] << std::endl;
            dataObj.dataPublish(tempSensor, s[5]);
        }
        catch (...) {
            std::cout << ".UDPclient.  Failed to Send Message" << std::endl;
        }

        std::cout << std::endl;
        
        try {
            std::cout << ".UDPclient.  Sending GET request for sensor: " << humSensor << std::endl;
            std::vector<char> returnMsg = sendMessage(toCoAP(hum));

            std::vector<std::string> s = fromCoAP(returnMsg);
            /*for (std::string str : s){
                std::cout << str << std::endl;
            }*/
            std::cout << ".UDPclient.  Received from " << humSensor << ": " << s[5] << std::endl;
            dataObj.dataPublish(humSensor, s[5]);
        }
        catch (...) {
            std::cout << "Failed to Send Message" << std::endl;
        }

        std::cout << "\n.UDPclient.  Waiting 9 seconds...\n" << std::endl;
        Sleep(9000);

    }

    return;

}

std::vector<std::string> createGetMessage(std::string OptionValue){
    //Get Message for Sensor 
    std::vector<std::string> msg;
        
    std::string ver = "1";
    //Ver is [0]
    msg.push_back(ver); // CoAP version: (01) is version 1
    std::string T = "NON";
    //T is [1]
    msg.push_back(T); // Message type: CON (00), NON (01), ACK (10) and RST (11)

    std::string code = "GET";
    //code is [2]
    msg.push_back(code); // Method: Empty (0000 0000), GET (0000 0001), POST (0000 0010), PUT (0000 0011), DELETE (0000 0100)
    
    std::string Token = "";
    //Token is [3]
    msg.push_back(Token); // Token
    
    std::string OptionDelta = "Uri-Path";
    //OptionDelta is [4]
    msg.push_back(OptionDelta); // Options
    
    //OptionValue is [5]
    msg.push_back(OptionValue); // Options
    
    std::string Payload = "test";
    //Payload is [6]
    msg.push_back(Payload); // Payload
    //----------------------------------

    return msg;
}