#include "msgHandler.h"

#include <iostream>
#include <fstream>
#include <algorithm>

std::vector<std::string> SplitHeader(std::string given_str){
    std::vector<std::string> headerSplit; 

    headerSplit.push_back(given_str.substr(0, 4)); // store the substring
    headerSplit.push_back(given_str.substr(4, 1)); // store the substring
    headerSplit.push_back(given_str.substr(5, 2)); // store the substring
    headerSplit.push_back(given_str.substr(7, 1)); // store the substring
    headerSplit.push_back(given_str.substr(8, 8)); // store the substring


    return headerSplit;
}

int binToDec(std::string binNum){
    long bin, dec = 0, rem, num, base = 1;
    num = std::stoi(binNum);
    bin = num;
    while (num > 0)
    {
        rem = num % 10;
        dec = dec + rem * base;
        base = base * 2;
        num = num / 10;
    }
    //std::cout << "The decimal equivalent of " << bin << " : " << dec << std::endl;
    return dec;
}

std::string HandleRequest(std::string request, std::string given_str){

    int requestNr = binToDec(request);

    switch(requestNr){
        case 0:
            //RESERVED
            break;
        case 1:
            //CONNECT
            break;
        case 2:
            //CONNACK
            break;
        case 3:
            //PUBLISH
            break;
        case 4:
            //PUBACK
            break;
        case 5:
            //PUBREC
            break;
        case 6:
            //PUBREL
            break;
        case 7:
            //PUBCOMP
            break;
        case 8:
            //SUBSCRIBE
            break;
        case 9:
            //SUBACK
            break;
        case 10:
            //UNSUBSCRIBE
            break;
        case 11:
            //UNSUBACK
            break;
        case 12:
            //PINGREQ
            break;
        case 13:
            //PINGRESP
            break;
        case 14:
            //DISCONNECT
            break;
        case 15:
            //RESERVED
            break;
    }

    return "TEST";
}

std::vector<std::vector<std::string>> readDatabase(){
    
    std::vector<std::vector<std::string>> storedData;
    int databaseSize = 0;
    std::string delimiter = ": ";
    std::string line;
    std::ifstream myfile ("database.txt");
    if (myfile.is_open())
    {
        while ( std::getline (myfile,line) )
        { 
            std::string lineCopy = line;
            size_t pos = lineCopy.find (delimiter);

            if( pos != std::string::npos)  
            {  
                storedData.push_back(std::vector<std::string>());
                storedData[storedData.size()-1].push_back(lineCopy.substr(0, pos)); // store the substring  
                lineCopy.erase(0, pos + delimiter.length()); // erase previous substring  
            }  
            
            storedData[storedData.size()-1].push_back(delimiter);
            storedData[storedData.size()-1].push_back(lineCopy);
            databaseSize++;
        }
        myfile.close();
    }
    else{
        std::cout << "File could not be read" << std::endl;
    }

    return storedData;
}

void updateDatabase(std::vector<std::vector<std::string>> storedData){

    std::ofstream myfile1 ("database.txt");
    myfile1.open("database.txt", std::ofstream::out | std::ofstream::trunc);
    myfile1.close();
    std::ofstream myfile2 ("database.txt");
    if (myfile2.is_open())
    {
        for (int i = 0; i < storedData.size(); i++)
        {
            myfile2 << storedData[i][0] + storedData[i][1] + storedData[i][2] + "\n";
        }
        myfile2.close();
    }
}