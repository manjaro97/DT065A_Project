#ifndef CONVERTCOAP_H
#define CONVERTCOAP_H

#include <string>
#include <vector>

std::vector<char> toCoAP(std::vector<std::string>);

std::vector<std::string> fromCoAP(std::vector<char>);

#endif