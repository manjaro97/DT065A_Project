#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> SplitHeaderBody(std::string given_str, std::string delimiter);

int main(){
    std::string given_str = "GET /Address/ HTTP/1.1/\r\nHeader Stuff\r\nDoc-TYPE = lol\r\n\r\nBody - Testing hoping this shit works sometime";
    std::vector<std::string> msgSplit = SplitHeaderBody(given_str, "\r\n\r\n");

    std::vector<std::string> headerSplit = SplitHeaderBody(msgSplit[0], "\r\n");

    std::vector<std::string> requestSplit = SplitHeaderBody(headerSplit[0], " ");
    
    std::cout << "end" << std::endl;
}

std::vector<std::string> SplitHeaderBody(std::string given_str, std::string delimiter){

    std::vector<std::string> msgSplit; 
    size_t pos = 0;

    while (( pos = given_str.find (delimiter)) != std::string::npos)  
    {  
        msgSplit.push_back(given_str.substr(0, pos)); // store the substring  
        given_str.erase(0, pos + delimiter.length()); // erase previous substring   
    }  
    msgSplit.push_back(given_str);

    return msgSplit;
}