#include "ConnectHandler.h"

#include "TCPlistener.h"

#include <bitset>
#include <vector>
#include <iostream>


std::vector<char> DecodeConnect(std::string msg){
    
    std::cout << "Decoding Connect Message" << std::endl;

    //Header
    msg.erase(0, 8);
    //VariableLength
    int remainingLength = std::bitset<8>(msg.substr(0, 8)).to_ulong();
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
    int version = std::bitset<8>(msg.substr(0, 8)).to_ulong();
    msg.erase(0, 8);
    //Connect Flags
    std::string connectFlags = msg.substr(0, 8);
    msg.erase(0, 8);
    //Time to keep connection alive without response
    int keepAlive = std::bitset<16>(msg.substr(0, 16)).to_ulong();
    msg.erase(0, 16);

    std::cout << "End of Decoding Connect Message" << std::endl;
    return SendConnAck();
}

std::vector<char> SendConnAck(){

    std::cout << "Sending ConnAck Message" << std::endl;

    std::vector<char> byteVector;
    
    //Header
    std::string returnHeader = "00100000";
    byteVector.push_back(std::bitset<8>(returnHeader.substr(0, 8)).to_ulong());

    std::string returnStr;

    //Input  

    //Connect Acknowledge Flags
    returnStr += "00000000";  // Could be "00000001"

    //Connect Reason Code
    returnStr += "00000000";  //Success

    //Property Length
    byteVector.push_back(std::bitset<8>(returnStr.length()/8).to_ulong());
    
    for(int i = 0; i < returnStr.length()/8; i++){
        byteVector.push_back(std::bitset<8>(returnStr.substr(i*0, i*8)).to_ulong());
    }

    std::cout << "End of Sending ConnAck Message" << std::endl;

    return byteVector;
}

std::vector<char> DecodeDisconnect(SOCKET clientSOCKET){
    
    databaseObj.DisconnectEraseAll(clientSOCKET);

    std::vector<char> emptyResponse;
    return emptyResponse;
}