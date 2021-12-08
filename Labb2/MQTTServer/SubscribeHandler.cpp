#include "SubscribeHandler.h"
#include <iostream>
#include <bitset>

std::vector<char> DecodeSubscribe(std::string msg){
    //TODO: FIX THIS CODE
    std::cout << "Decoding Subscibe Message" << std::endl;

    //Header
    msg.erase(0, 8);
    //VariableLength
    int lengthVariableHeader = std::bitset<8>(msg.substr(0, 8)).to_ulong();
    std::cout << lengthVariableHeader << std::endl;
    msg.erase(0, 8);

    //Package Identifier
    int packageID = std::bitset<16>(msg.substr(0, 16)).to_ulong();
    std::cout  << "packageID: " << packageID << std::endl;
    msg.erase(0, 16);
    lengthVariableHeader -= 16;

    //Payload
    int bodyLength;
    std::string body;
    int i = 0;
    char c;
    int QoS;
    while(i < lengthVariableHeader)
    {
        bodyLength = std::bitset<16>(msg.substr(0, 16)).to_ulong();
        std::cout << "bodyLength: " << bodyLength << std::endl;
        msg.erase(0, 16);
        i+=16;
        body = "";
        while(std::bitset<8>(msg.substr(0, 6)).to_string() != "000000"){
            c = char(std::bitset<8>(msg.substr(0, 8)).to_ulong());
            std::cout << "c: " << c << std::endl;
            body += c;
            msg.erase(0, 8);
            i+=8;
        }
        //Body Ender
        msg.erase(0, 6);
        QoS = std::bitset<8>(msg.substr(0, 2)).to_ulong();
        std::cout  << "QoS: " << QoS << std::endl;
        i+=8;
    }
    
    std::cout << "End of Decoding Subscibe Message" << std::endl;
    return SendSubAck();
}

std::vector<char> SendSubAck(){
    std::cout << "Sending SubAck Message" << std::endl;

    std::vector<char> byteVector;

    

    std::cout << "End of Sending SubAck Message" << std::endl;
    return byteVector;
}

std::vector<char> DecodeUnsubscribe(std::string msg){
    return SendUnsubAck();
}

std::vector<char> SendUnsubAck(){
    std::vector<char> bytesVector;
    return bytesVector;
}