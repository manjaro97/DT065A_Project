#ifndef CONNECTHANDLER_H
#define CONNECTHANDLER_H

#include "TCPlistener.h"
#include "Header.h"
#include "database.h"

#include <string>

std::vector<char> DecodeConnect(std::string msg);

std::vector<char> SendConnAck();

std::vector<char> DecodeDisconnect(SOCKET clientSOCKET);

#endif