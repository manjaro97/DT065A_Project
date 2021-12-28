#include "ConnectHandler.h"

#include <bitset>
#include <iostream>


std::vector<char> SendConnect(){
    
    std::cout << "Sending Connect Message" << std::endl;

    std::vector<char> sendMsg;

    //Header
    sendMsg.push_back(char(std::bitset<8>("00010000").to_ulong()));

    std::string strBuffer = "";
    
    std::string protocolName = "MQTT";

    //Length of Protocol Name
    strBuffer += std::bitset<16>(protocolName.length()).to_string();

    //Translate Protocol Name
    for(char c: protocolName){
        strBuffer += std::bitset<8>(c).to_string();
    }
    
    //Version Number
    strBuffer += std::bitset<8>(4).to_string();

    //Connect Flags
    strBuffer += "00000000";

    //Time to keep connection alive without response
    strBuffer += std::bitset<16>(300).to_string();

    //VariableLength
    sendMsg.push_back(std::bitset<8>(strBuffer.length()/8).to_ulong());

    for (int i = 0; i < (strBuffer.length()/8); i++)
    {
        sendMsg.push_back(std::bitset<8>(strBuffer.substr(i, 8)).to_ulong());
    }

    std::cout << "End of Sending Connect Message" << std::endl;
    return sendMsg;
}

std::vector<char> DecodeConnAck(){

    std::cout << "Decoding ConnAck Message" << std::endl;

    //TODO: Not needed for this project

    std::vector<char> emptyResponse;
    return emptyResponse;
}

std::vector<char> SendDisconnect(){
    
    std::cout << "Sending Disconnect Message" << std::endl;

    std::vector<char> sendMsg;

    //Header
    sendMsg.push_back(char(std::bitset<8>("11100000").to_ulong()));
  
    sendMsg.push_back(char(std::bitset<8>("00000000").to_ulong()));

    std::cout << "End of Sending Disconnect Message" << std::endl;
    return sendMsg;
   
}