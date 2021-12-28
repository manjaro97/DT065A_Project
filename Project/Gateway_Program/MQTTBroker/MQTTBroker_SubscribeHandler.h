#ifndef MSUBSCRIBEHANDLER_H
#define MSUBSCRIBEHANDLER_H

#include "MQTTBroker_TCPlistener.h"
#include "MQTTBroker_Header.h"
#include <string>
#include <vector>

std::vector<char> DecodeSubscribe(std::string msg, SOCKET clientSOCKET);

std::vector<char> SendSubAck(std::string subAckMsg);

std::vector<char> DecodeUnsubscribe(std::string msg, SOCKET clientSOCKET);

std::vector<char> SendUnsubAck(std::string subAckMsg);

#endif