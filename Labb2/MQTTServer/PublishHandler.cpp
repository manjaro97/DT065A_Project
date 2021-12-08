#include "PublishHandler.h"
#include <iostream>
#include <bitset>

std::vector<char> DecodePublish(std::string msg){
    
    std::cout << "Decoding Publish Message" << std::endl;

    //Header
    int QoS = std::bitset<8>(msg.substr(5, 2)).to_ulong();
    msg.erase(0, 8);
    std::cout << "QoS: " << QoS << std::endl;

    //Remaining Length
    int remainingLength = std::bitset<8>(msg.substr(0, 8)).to_ulong();
    msg.erase(0, 8);
    remainingLength -= 2;
    std::cout << "remainingLength: " << remainingLength << std::endl;

    //Topic Length
    int topicLength = std::bitset<16>(msg.substr(0, 16)).to_ulong();
    msg.erase(0, 16);
    std::cout << "topicLength: " << topicLength << std::endl;

    std::string topic = "";
    char c;
    for (int i2 = 0; i2 < topicLength; i2++)
    {
        c = char(std::bitset<8>(msg.substr(0, 8)).to_ulong());
        topic += c;
        msg.erase(0, 8);
    }
    
    std::string packetID;
    if(QoS != 0){
        //Package Identifier
        int packageID = std::bitset<16>(msg.substr(0, 16)).to_ulong();
        packetID = msg.substr(0, 16);
        msg.erase(0, 16);
        std::cout << "Package ID: " << packageID << std::endl;
    }

    //Handle Payload
    std::string message = "";
    if(msg.length() != 0){
        for (int i = 0; i < msg.length()/8; i++)
        {
            message += char(std::bitset<8>(msg.substr(i*8, 8)).to_ulong());
        }
    }    

    //TODO: Publish topic & message to Database
    
    std::cout << "End of Decoding Publish Message" << std::endl;
    std::vector<char> emptyResponse;
    switch(QoS){
        case 0:
            return emptyResponse;
        case 1:
            return SendPubAck(packetID);
        case 2:
            return SendPubRec(packetID);
        default:
            std::cout << "Publish QoS ERROR"<< std::endl;
            break;
    }
    return emptyResponse;
}

std::vector<char> SendPublish(int QoS, std::string topic, std::string body){ 
    
    std::cout << "Sending Publish Message" << std::endl;

    std::vector<char> byteVector;

    //Header
    std::string returnHeader = "0011";
    returnHeader += "0";//DUP flag
    returnHeader += std::bitset<2>(QoS).to_string();
    returnHeader += "0";
    byteVector.push_back(std::bitset<8>(returnHeader.substr(0, 8)).to_ulong());

    std::string variable = "";

    //Topic Length
    variable += std::bitset<16>(topic.length()).to_ulong();

    //Topic
    for (int i = 0; i < topic.length(); i++)
    {
        variable += std::bitset<8>(topic[i]).to_ulong();
    }

    //Packet ID
    if(QoS != 0){
        int packageID = rand() % 100000;
        variable += std::bitset<16>(packageID).to_ulong();
    }

    //Body
    for (int i = 0; i < body.length(); i++)
    {
        variable += std::bitset<8>(body[i]).to_ulong();
    }

    //Remaining Length
    byteVector.push_back(std::bitset<8>(variable.length()/8).to_ulong());

    //Push_back of rest
    for (int i = 0; i < variable.length()/8; i++)
    {
        byteVector.push_back(std::bitset<8>(variable.substr(i*8, 8)).to_ulong());
    }

    std::cout << "End of Sending Publish Message" << std::endl;

    return byteVector;
}

std::vector<char> DecodePubAck(std::string msg){
    
    std::cout << "Decoding PubAck Message" << std::endl;

    //Header
    msg.erase(0, 8);

    //VariableLength
    int remainingLength = std::bitset<8>(msg.substr(0, 8)).to_ulong();
    msg.erase(0, 8);

    //Package Identifier
    int packageID = std::bitset<16>(msg.substr(0, 16)).to_ulong();
    std::string packetID = msg.substr(0, 16);
    msg.erase(0, 16);
    
    std::cout << "End of Decoding PubAck Message" << std::endl;

    std::vector<char> emptyResponse;
    return emptyResponse;
}

std::vector<char> SendPubAck(std::string packetID){

    std::cout << "Sending PubAck Message" << std::endl;

    std::vector<char> byteVector;

    //Header
    std::string returnHeader = "01000000";
    byteVector.push_back(std::bitset<8>(returnHeader.substr(0, 8)).to_ulong());

    //Remaining Length
    byteVector.push_back(std::bitset<8>(packetID.length()/8).to_ulong());

    //Packet ID
    for (int i = 0; i < packetID.length()/8; i++)
    {
        byteVector.push_back(std::bitset<8>(packetID.substr(i*8, 8)).to_ulong());
    }

    std::cout << "End of Sending PubAck Message" << std::endl;

    return byteVector;
}

std::vector<char> DecodePubRec(std::string msg){

    std::cout << "Decoding PubRec Message" << std::endl;

    //Header
    msg.erase(0, 8);

    //VariableLength
    int remainingLength = std::bitset<8>(msg.substr(0, 8)).to_ulong();
    msg.erase(0, 8);

    //Package Identifier
    int packageID = std::bitset<16>(msg.substr(0, 16)).to_ulong();
    std::string packetID = msg.substr(0, 16);
    msg.erase(0, 16);
    
    std::cout << "End of Decoding PubRec Message" << std::endl;

    return SendPubRel(packetID);

}

std::vector<char> SendPubRec(std::string packetID){ 
    
    std::cout << "Sending PubRec Message" << std::endl;

    std::vector<char> byteVector;

    //Header
    std::string returnHeader = "01010000";
    byteVector.push_back(std::bitset<8>(returnHeader.substr(0, 8)).to_ulong());

    //Remaining Length
    byteVector.push_back(std::bitset<8>(packetID.length()/8).to_ulong());

    //Packet ID
    for (int i = 0; i < packetID.length()/8; i++)
    {
        byteVector.push_back(std::bitset<8>(packetID.substr(i*8, 8)).to_ulong());
    }

    std::cout << "End of Sending PubRec Message" << std::endl;

    return byteVector;
}

std::vector<char> DecodePubRel(std::string msg){
    
    std::cout << "Decoding PubRel Message" << std::endl;

    //Header
    if(msg.substr(4, 4) != "0010"){
        std::cout << "ERROR in Received PubRel" << std::endl;
    }
    msg.erase(0, 8);

    //VariableLength
    int remainingLength = std::bitset<8>(msg.substr(0, 8)).to_ulong();
    msg.erase(0, 8);

    //Package Identifier
    int packageID = std::bitset<16>(msg.substr(0, 16)).to_ulong();
    std::string packetID = msg.substr(0, 16);
    msg.erase(0, 16);
        
    std::cout << "End of Decoding PubRel Message" << std::endl;

    return SendPubComp(packetID);
    
}

std::vector<char> SendPubRel(std::string packetID){
    
    std::cout << "Sending PubRel Message" << std::endl;

    std::vector<char> byteVector;

    //Header
    std::string returnHeader = "01100010";
    byteVector.push_back(std::bitset<8>(returnHeader.substr(0, 8)).to_ulong());

    //Remaining Length
    byteVector.push_back(std::bitset<8>(packetID.length()/8).to_ulong());

    //Packet ID
    for (int i = 0; i < packetID.length()/8; i++)
    {
        byteVector.push_back(std::bitset<8>(packetID.substr(i*8, 8)).to_ulong());
    }

    std::cout << "End of Sending PubRel Message" << std::endl;

    return byteVector;
}

std::vector<char> DecodePubComp(std::string msg){
    
    std::cout << "Decoding PubComp Message" << std::endl;

    //Header
    msg.erase(0, 8);

    //VariableLength
    int remainingLength = std::bitset<8>(msg.substr(0, 8)).to_ulong();
    msg.erase(0, 8);

    //Package Identifier
    int packageID = std::bitset<16>(msg.substr(0, 16)).to_ulong();
    std::string packetID = msg.substr(0, 16);
    msg.erase(0, 16);
    
    std::cout << "End of Decoding PubComp Message" << std::endl;

    std::vector<char> emptyResponse;
    return emptyResponse;
}

std::vector<char> SendPubComp(std::string packetID){ 
    
    std::cout << "Sending PubComp Message" << std::endl;

    std::vector<char> byteVector;

    //Header
    std::string returnHeader = "01110000";
    byteVector.push_back(std::bitset<8>(returnHeader.substr(0, 8)).to_ulong());

    //Remaining Length
    byteVector.push_back(std::bitset<8>(packetID.length()/8).to_ulong());

    //Packet ID
    for (int i = 0; i < packetID.length()/8; i++)
    {
        byteVector.push_back(std::bitset<8>(packetID.substr(i*8, 8)).to_ulong());
    }

    std::cout << "End of Sending PubComp Message" << std::endl;

    return byteVector;
}