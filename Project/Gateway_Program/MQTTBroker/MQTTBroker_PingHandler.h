#ifndef MPINGHANDLER_H
#define MPINGHANDLER_H

#include <string>
#include <vector>

std::vector<char> DecodePingReq(std::string msg);

std::vector<char> SendPingAck();

#endif