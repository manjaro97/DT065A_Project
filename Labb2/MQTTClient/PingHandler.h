#ifndef PINGHANDLER_H
#define PINGHANDLER_H

#include <string>
#include <vector>

std::vector<char> SendPingReq();

std::vector<char> DecodePingResp(std::string msg);

#endif