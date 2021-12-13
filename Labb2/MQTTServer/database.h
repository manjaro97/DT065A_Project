#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <string>

std::vector<std::vector<std::string>> readDatabase();

void updateDatabase(std::vector<std::vector<std::string>> storedData);

#endif

