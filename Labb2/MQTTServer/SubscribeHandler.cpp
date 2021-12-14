#include "SubscribeHandler.h"
#include <iostream>
#include <bitset>

std::vector<char> DecodeSubscribe(std::string msg, SOCKET clientSOCKET){
    
    std::cout << "Decoding Subscibe Message" << std::endl;

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
        //Topic Ender
        msg.erase(0, 6);
        QoS = std::bitset<8>(msg.substr(0, 2)).to_ulong();
        msg.erase(0, 2);
        i+=1;

        std::cout << "Adding Subscription" << std::endl;
        //Send SOCKET and topic to database
        databaseObj.AddSubscription(topic, clientSOCKET);

        switch (QoS){
        case 0:
            subAckMsg += "00000000";
            break;
        case 1:
            subAckMsg += "00000001";
            break;
        case 2:
            subAckMsg += "00000010";
            break;
        }

    }
    
    std::cout << "End of Decoding Subscibe Message" << std::endl;
    return SendSubAck(subAckMsg);
}

std::vector<char> SendSubAck(std::string subAckMsg){
    std::cout << "Sending SubAck Message" << std::endl;

    std::vector<char> byteVector;

    //Header
    std::string returnHeader = "10010000";
    byteVector.push_back(std::bitset<8>(returnHeader.substr(0, 8)).to_ulong());

    //Remaining Length
    byteVector.push_back(std::bitset<8>(subAckMsg.length()/8).to_ulong());

    for (int i = 0; i < subAckMsg.length()/8; i++)
    {
        byteVector.push_back(std::bitset<8>(subAckMsg.substr(i*8, 8)).to_ulong());
    }

    std::cout << "End of Sending SubAck Message" << std::endl;
    return byteVector;
}

std::vector<char> DecodeUnsubscribe(std::string msg, SOCKET clientSOCKET){
    
    std::cout << "Decoding Subscibe Message" << std::endl;

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

std::vector<char> SendUnsubAck(std::string subAckMsg){
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