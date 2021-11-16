#ifndef CONVERTCOAP_H
#define CONVERTCOAP_H

#include <string>
#include <vector>

std::string toCoAP(std::vector<std::string>);

std::vector<std::string> fromCoAP(std::string);

int binToDec(std::string);

#endif