#include "MQTTClient_PublishHandler.h"
#include <iostream>
#include <bitset>

std::vector<char> clientSendPublish(int QoS, std::string topic, std::string body){ 
    
    std::cout << ".MQTTclient. Sending Publish Message" << std::endl;

    std::vector<char> byteVector;

    //Header
    std::string returnHeader = "0011";
    returnHeader += "0";//DUP flag
    returnHeader += std::bitset<2>(QoS).to_string();
    returnHeader += "0";
    byteVector.push_back(std::bitset<8>(returnHeader.substr(0, 8)).to_ulong());

    std::string variable = "";

    //Topic Length
    variable += std::bitset<16>(topic.length()).to_string();

    //Topic
    for (int i = 0; i < topic.length(); i++)
    {
        variable += std::bitset<8>(topic[i]).to_string();
    }

    //Packet ID
    if(QoS != 0){
        int packageID = rand() % 100000;
        variable += std::bitset<16>(packageID).to_string();
    }

    //Body
    for (int i = 0; i < body.length(); i++)
    {
        variable += std::bitset<8>(body[i]).to_string();
    }

    //Remaining Length
    byteVector.push_back(std::bitset<8>(variable.length()/8).to_ulong());

    //Push_back of rest
    for (int i = 0; i < variable.length()/8; i++)
    {
        byteVector.push_back(std::bitset<8>(variable.substr(i*8, 8)).to_ulong());
    }

    std::cout << ".MQTTclient. End of Sending Publish Message" << std::endl;

    return byteVector;
}
