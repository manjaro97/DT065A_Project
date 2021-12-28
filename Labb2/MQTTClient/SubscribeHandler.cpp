#include "SubscribeHandler.h"
#include <iostream>
#include <bitset>
#include <random>

std::vector<char> SendSubscribe(std::string topic){
    
    std::cout << "Sending Subscribe Message" << std::endl;

    std::vector<char> byteVector;

    //Header
    std::string returnHeader = "10000000";
    byteVector.push_back(std::bitset<8>(returnHeader.substr(0, 8)).to_ulong());

    //VariableLength
    int remainingLength;
    std::string variable = "";

    //Package Identifier
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(1,100000); // distribution in range [1, 6]

    int packageID = dist(rng);
    variable += std::bitset<16>(packageID).to_string();

    //Payload
    for(char c: topic){
        variable += std::bitset<8>(packageID).to_string();
    }

    //pushback Variable Length
    remainingLength = variable.length()/8;
    byteVector.push_back(std::bitset<8>(remainingLength).to_ulong());

    //pushback rest
    for (int i = 0; i < remainingLength; i++)
    {
        byteVector.push_back(std::bitset<8>(variable.substr(0, 8)).to_ulong());
        variable.erase(0, 8);
    }

    std::cout << "End of Sending Subscribe Message" << std::endl;
    return byteVector;
}

std::vector<char> DecodeSubAck(std::string msg){
    std::cout << "Decoding SubAck Message" << std::endl;

    //Header
    msg.erase(0, 8);

    //VariableLength
    int remainingLength = std::bitset<8>(msg.substr(0, 8)).to_ulong();
    msg.erase(0, 8);

    //Package Identifier
    int packageID = std::bitset<16>(msg.substr(0, 16)).to_ulong();
    std::string subAckMsg = msg.substr(0, 16);
    msg.erase(0, 16);
    remainingLength -= 2;

    //Payload
    int topicLength;
    std::string topic;
    int i = 0;
    char c;
    int QoS;
    while(i < remainingLength)
    {
        topicLength = std::bitset<16>(msg.substr(0, 16)).to_ulong();
        msg.erase(0, 16);
        i+=2;
        topic = "";
        for (int i2 = 0; i2 < topicLength; i2++)
        {
            c = char(std::bitset<8>(msg.substr(0, 8)).to_ulong());
            topic += c;
            msg.erase(0, 8);
            i+=1;
        }
        
        std::cout << "Deleting Subscription" << std::endl;
        //Remove SOCKET and topic from database
        databaseObj.RemoveSubscription(topic, clientSOCKET);

    }
    
    std::cout << "End of Decoding Unsubscibe Message" << std::endl;

    return SendUnsubAck(subAckMsg);
}

std::vector<char> SendUnsubscribe(std::string msg, SOCKET clientSOCKET){
    
    
}

std::vector<char> DecodeUnsubAck(std::string subAckMsg){
    std::cout << "Sending UnsubAck Message" << std::endl;

    std::vector<char> byteVector;

    //Header
    std::string returnHeader = "10110000";
    byteVector.push_back(std::bitset<8>(returnHeader.substr(0, 8)).to_ulong());

    //Remaining Length
    byteVector.push_back(std::bitset<8>(subAckMsg.length()/8).to_ulong());

    for (int i = 0; i < subAckMsg.length()/8; i++)
    {
        byteVector.push_back(std::bitset<8>(subAckMsg.substr(i*8, 8)).to_ulong());
    }

    std::cout << "End of Sending UnsubAck Message" << std::endl;
    return byteVector;
}