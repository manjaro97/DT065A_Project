#ifndef SUBSCRIBEHANDLER_H
#define SUBSCRIBEHANDLER_H

#include <string>
#include <vector>

std::vector<char> DecodeSubscribe(std::string msg);

std::vector<char> SendSubAck();

std::vector<char> DecodeUnsubscribe(std::string msg);

std::vector<char> SendUnsubAck();

#endif