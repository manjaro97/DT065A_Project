#include "MQTTBroker_PingHandler.h"
#include <iostream>
#include <bitset>

std::vector<char> DecodePingReq(std::string msg){
    std::cout << "Decoding Ping Request" << std::endl;

    //Header
    msg.erase(0, 8);
    //VariableLength
    int remainingLength = std::bitset<8>(msg.substr(0, 8)).to_ulong();
    msg.erase(0, 8);

    std::cout << "End of Decoding Ping Request" << std::endl;
    return SendPingAck();
}

std::vector<char> SendPingAck(){
    std::cout << "Sending PingAck Message" << std::endl;

    std::vector<char> byteVector;
    
    //Header
    std::string returnHeader = "11010000";
    byteVector.push_back(std::bitset<8>(returnHeader.substr(0, 8)).to_ulong());

    std::string returnStr = "";

    //Property Length
    byteVector.push_back(std::bitset<8>(returnStr.length()/8).to_ulong());

    std::cout << "End of Sending PingAck Message" << std::endl;

    return byteVector;
}