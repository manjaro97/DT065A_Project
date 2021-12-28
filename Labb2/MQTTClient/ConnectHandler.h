#ifndef CONNECTHANDLER_H
#define CONNECTHANDLER_H

#include <string>
#include <vector>

std::vector<char> SendConnect();

std::vector<char> DecodeConnAck();

std::vector<char> SendDisconnect();

#endif