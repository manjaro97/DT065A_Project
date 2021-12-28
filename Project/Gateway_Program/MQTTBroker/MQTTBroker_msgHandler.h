#ifndef MMSGHANDLER_H
#define MMSGHANDLER_H

#include <string>
#include <vector>
#include <WS2tcpip.h>

std::vector<char> HandleRequest(std::string given_str, SOCKET clientSOCKET);

#endif