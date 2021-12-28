#ifndef MPUBLISHHANDLER_H
#define MPUBLISHHANDLER_H

#include <vector>
#include <string>

std::vector<char> DecodePublish(std::string msg);

std::vector<char> SendPublish(int QoS, std::string topic, std::string body);

std::vector<char> DecodePubAck(std::string msg);

std::vector<char> SendPubAck(std::string packetID);

std::vector<char> DecodePubRec(std::string msg);

std::vector<char> SendPubRec(std::string packetID);

std::vector<char> DecodePubRel(std::string msg);

std::vector<char> SendPubRel(std::string packetID);

std::vector<char> DecodePubComp(std::string msg);

std::vector<char> SendPubComp(std::string packetID);


#endif