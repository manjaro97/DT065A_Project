#include <iostream>
#include <bitset>
#include "convertCoAP.h"

std::vector<char> toCoAP(std::vector<std::string> inputMsg){
    
    std::vector<char> msgCoAP;

    // Version conversion
    if(inputMsg[0] == "1"){
        msgCoAP.push_back('0');
        msgCoAP.push_back('1');
    }
    else{
        std::cout << "ERROR: Invalid Version" << std::endl;
        return msgCoAP;
    }

    // Message type conversion
    if(inputMsg[1] == "CON"){
        msgCoAP.push_back('0');
        msgCoAP.push_back('0');
    }
    else if(inputMsg[1] == "NON"){
        msgCoAP.push_back('0');
        msgCoAP.push_back('1');
    }
    else if(inputMsg[1] == "ACK"){
        msgCoAP.push_back('1');
        msgCoAP.push_back('0');
    }
    else if(inputMsg[1] == "RST"){
        msgCoAP.push_back('1');
        msgCoAP.push_back('1');
    }
    else{
        std::cout << "ERROR: Invalid Message Type T" << std::endl;
        return msgCoAP;
    }

    // TKL conversion
    if(inputMsg[3].length() == 0){
        msgCoAP.push_back('0');
        msgCoAP.push_back('0');
        msgCoAP.push_back('0');
        msgCoAP.push_back('0');
    }
    //TODO: FIX THIS
    int TKL = inputMsg[3].length();
        
    // Request Conversion
    if(inputMsg[2] == "EMPTY"){
        msgCoAP.push_back('0');
        msgCoAP.push_back('0');
        msgCoAP.push_back('0');
        msgCoAP.push_back('0');
        msgCoAP.push_back('0');
        msgCoAP.push_back('0');
        msgCoAP.push_back('0');
        msgCoAP.push_back('0');
    }
    else if(inputMsg[2] == "GET"){
        msgCoAP.push_back('0');
        msgCoAP.push_back('0');
        msgCoAP.push_back('0');
        msgCoAP.push_back('0');
        msgCoAP.push_back('0');
        msgCoAP.push_back('0');
        msgCoAP.push_back('0');
        msgCoAP.push_back('1');
    }
    else if(inputMsg[2] == "POST"){
        msgCoAP.push_back('0');
        msgCoAP.push_back('0');
        msgCoAP.push_back('0');
        msgCoAP.push_back('0');
        msgCoAP.push_back('0');
        msgCoAP.push_back('0');
        msgCoAP.push_back('1');
        msgCoAP.push_back('0');
    }
    else if(inputMsg[2] == "PUT"){
        msgCoAP.push_back('0');
        msgCoAP.push_back('0');
        msgCoAP.push_back('0');
        msgCoAP.push_back('0');
        msgCoAP.push_back('0');
        msgCoAP.push_back('0');
        msgCoAP.push_back('1');
        msgCoAP.push_back('1');
    }
    else if(inputMsg[2] == "DELETE"){
        msgCoAP.push_back('0');
        msgCoAP.push_back('0');
        msgCoAP.push_back('0');
        msgCoAP.push_back('0');
        msgCoAP.push_back('0');
        msgCoAP.push_back('1');
        msgCoAP.push_back('0');
        msgCoAP.push_back('0');
    }
    else{
        std::cout << "ERROR: Invalid Request" << std::endl;
        return msgCoAP;
    }

    //Create Random MEssage ID
    msgCoAP.push_back('0');
    msgCoAP.push_back('1');
    msgCoAP.push_back('0');
    msgCoAP.push_back('0');
    msgCoAP.push_back('0');
    msgCoAP.push_back('1');
    msgCoAP.push_back('0');
    msgCoAP.push_back('0');
    msgCoAP.push_back('0');
    msgCoAP.push_back('1');
    msgCoAP.push_back('0');
    msgCoAP.push_back('0');
    msgCoAP.push_back('1');
    msgCoAP.push_back('1');
    msgCoAP.push_back('0');
    msgCoAP.push_back('0');

    // Token ceonversion
    std::string token =  inputMsg[3];

    // OptionDelta conversion
    if(inputMsg[4] == "Uri-Path"){
        msgCoAP.push_back('1');
        msgCoAP.push_back('0');
        msgCoAP.push_back('1');
        msgCoAP.push_back('1');
    }
    else{
        std::cout << "ERROR: Invalid Option Delta" << std::endl;
        return msgCoAP;
    }

    // Option length
    int optionlength = inputMsg[5].length();
    std::string s = std::bitset< 4 >( optionlength ).to_string();
    for(char b: s){
        msgCoAP.push_back(b);
    }

    // Option Delta (extended)

    // Option length (extended) 

    // Option Value
    std::string str1 = inputMsg[5];
    std::vector<char> charBody1(str1.begin(), str1.end());
    std::string bitStr1;

    for(char c: charBody1){
        bitStr1 = std::bitset<8>(c).to_string();
        for(char b: bitStr1){
            msgCoAP.push_back(b);
        }
    }

    // End Header
    msgCoAP.push_back('1');
    msgCoAP.push_back('1');
    msgCoAP.push_back('1');
    msgCoAP.push_back('1');
    msgCoAP.push_back('1');
    msgCoAP.push_back('1');
    msgCoAP.push_back('1');
    msgCoAP.push_back('1');

    // Body Conversion
    std::string str2 = inputMsg[6];
    std::vector<char> charBody2(str2.begin(), str2.end());
    std::string bitStr2;

    for(char c: charBody2){
        bitStr2 = std::bitset<8>(c).to_string();
        for(char b: bitStr2){
            msgCoAP.push_back(b);
        }
    }

    return msgCoAP;
}

std::vector<std::string> fromCoAP(std::vector<char>){
    std::vector<std::string> test;
    return test;

}

