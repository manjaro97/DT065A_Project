#ifndef PUBLISHHANDLER_H
#define PUBLISHHANDLER_H

#include <vector>
#include <string>

std::vector<char> clientSendPublish(int QoS, std::string topic, std::string body);

#endif