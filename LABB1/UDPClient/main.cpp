#include <iostream>
#include <string>
#include "sendMessage.h"

// cd "C:\Users\j_c_k\Desktop\DT065A_Project\DT065A_Project\LABB1\UDPClient\" ; if ($?) { g++ main.cpp sendMessage.cpp sendMessage.h -o main -lws2_32} ; if ($?) { .\main }

int main(){

    std::string msg;

    while(true){
        std::cout << "Write a message you want to send: ";
        std::getline(std::cin, msg);
        std::cout << sendMessage(msg) << std::endl;
    }

}