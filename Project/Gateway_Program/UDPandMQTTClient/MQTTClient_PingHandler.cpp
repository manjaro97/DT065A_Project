#include "MQTTClient_PingHandler.h"
#include <iostream>
#include <bitset>

std::vector<char> SendPingReq(){
    std::cout << "Sending Ping Request" << std::endl;

    std::vector<char> sendMsg;

    //Header
    sendMsg.push_back(char(std::bitset<8>("11000000").to_ulong()));
  
    sendMsg.push_back(char(std::bitset<8>("00000000").to_ulong()));

    std::cout << "End of Sending Ping Request" << std::endl;
    return sendMsg;
}

std::vector<char> DecodePingResp(std::string msg){
    std::cout << "Decoding PingResp Message" << std::endl;

    //TODO: Not needed for this project

    std::vector<char> emptyResponse;
    return emptyResponse;
}