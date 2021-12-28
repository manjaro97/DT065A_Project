#ifndef SUBSCRIBEHANDLER_H
#define SUBSCRIBEHANDLER_H

#include <string>
#include <vector>

std::vector<char> SendSubscribe(std::string msg, SOCKET clientSOCKET);

std::vector<char> DecodeSubAck(std::string msg);

std::vector<char> SendUnsubscribe(std::string msg, SOCKET clientSOCKET);

std::vector<char> DecodeUnsubAck(std::string subAckMsg);

#endif