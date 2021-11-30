#ifndef MSGHANDLER_H
#define MSGHANDLER_H

#include <string>
#include <vector>

std::vector<std::string> SplitHeader(std::string given_str);

std::vector<char> HandleRequest(std::string msgType, std::string given_str);

std::vector<std::vector<std::string>> readDatabase();

void updateDatabase(std::vector<std::vector<std::string>> storedData);

#endif