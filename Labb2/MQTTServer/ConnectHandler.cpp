#include "ConnectHandler.h"

#include "TCPlistener.h"

#include <bitset>
#include <vector>
#include <iostream>


std::vector<char> DecodeConnect(std::string msg){

    //Header
    msg.erase(0, 8);
    //VariableLength
    int lengthVariableHeader = std::bitset<16>(msg.substr(0, 8)).to_ulong();
    msg.erase(0, 8);
    //Length of Protocol Name
    int lengthProtocolName = std::bitset<16>(msg.substr(0, 16)).to_ulong();
    msg.erase(0, 16);
    //Translate Protocol Name
    std::string protocolName;
    for (int i = 0; i < lengthProtocolName; i++)
    {
        protocolName += char(std::bitset<8>(msg.substr(0, 8)).to_ulong());
        msg.erase(0, 8);
    }
    std::cout << protocolName << std::endl;
    //Version Number
    int version = std::bitset<16>(msg.substr(0, 8)).to_ulong();
    msg.erase(0, 8);
    //Connect Flags
    std::string connectFlags = msg.substr(0, 8);
    msg.erase(0, 8);
    //Time to keep connection alive without response
    int keepAlive = std::bitset<16>(msg.substr(0, 16)).to_ulong();
    msg.erase(0, 16);

    return SendConnAck();
}

std::vector<char> SendConnAck(){
    
    std::vector<char> returnMsg;

    
    returnMsg = {'0', '0', '0', '1', '0', '0', '0', '0'};
    return returnMsg;

}


void DecodeDisconnect(){
    
}