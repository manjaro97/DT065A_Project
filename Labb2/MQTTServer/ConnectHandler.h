#ifndef CONNECTHANDLER_H
#define CONNECTHANDLER_H

#include "TCPlistener.h"

#include <string>

std::vector<char> DecodeConnect(std::string msg);

std::vector<char> SendConnAck();

void DecodeDisconnect();

#endif