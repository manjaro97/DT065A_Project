#ifndef MCONNECTHANDLER_H
#define MCONNECTHANDLER_H

#include "MQTTBroker_TCPlistener.h"
#include "MQTTBroker_Header.h"
#include "MQTTBroker_database.h"

#include <string>

std::vector<char> DecodeConnect(std::string msg);

std::vector<char> SendConnAck();

std::vector<char> DecodeDisconnect(SOCKET clientSOCKET);

#endif