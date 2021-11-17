#include <iostream>
#include <bitset>
#include <sstream>
#include "convertCoAP.h"

int binToDec(std::string);
std::string binToText(std::string str);
std::string fromCoAPoptionDelta(std::string delta);
std::string toCoAPoptionDelta(std::string delta);

std::vector<char> toCoAP(std::vector<std::string> inputMsg){
    
    std::string msgCoAPstr = "";

    // Version conversion
    if(inputMsg[0] == "1"){
        msgCoAPstr = msgCoAPstr + "01";
    }
    else{
        std::cout << "ERROR: Invalid Version" << std::endl;
    }

    // Message type conversion
    if(inputMsg[1] == "CON"){
        msgCoAPstr = msgCoAPstr + "00";
    }
    else if(inputMsg[1] == "NON"){
        msgCoAPstr = msgCoAPstr + "01";
    }
    else if(inputMsg[1] == "ACK"){
        msgCoAPstr = msgCoAPstr + "10";
    }
    else if(inputMsg[1] == "RST"){
        msgCoAPstr = msgCoAPstr + "11";
    }
    else{
        std::cout << "ERROR: Invalid Message Type T" << std::endl;
    }

    // TKL conversion
    std::string token = "";
    if(inputMsg[3].length() == 0){
        msgCoAPstr += "0000";
    }
    else if (std::stoi(inputMsg[3]) <= 255){
        msgCoAPstr += "0001";
        token = std::bitset< 8 >( std::stoi(inputMsg[3]) ).to_string();
    }
    else if (std::stoi(inputMsg[3]) <= 65535){
        msgCoAPstr += "0010";
        token = std::bitset< 16 >( std::stoi(inputMsg[3]) ).to_string();
    }
    else if (std::stoi(inputMsg[3]) <= 16777215){
        msgCoAPstr += "0011";
        token = std::bitset< 24 >( std::stoi(inputMsg[3]) ).to_string();
    }
    else if (std::stoi(inputMsg[3]) <= 4294967295){
        msgCoAPstr += "0100";
        token = std::bitset< 32 >( std::stoi(inputMsg[3]) ).to_string();
    } 
    else{
        std::cout << "ERROR: Token to Large" << std::endl;
    }

    // Request Code Conversion
    if(inputMsg[2] == "EMPTY"){
        msgCoAPstr = msgCoAPstr + "00000000";
    }
    else if(inputMsg[2] == "GET"){
        msgCoAPstr = msgCoAPstr + "00000001";
    }
    else if(inputMsg[2] == "POST"){
        msgCoAPstr = msgCoAPstr + "00000010";
    }
    else if(inputMsg[2] == "PUT"){
        msgCoAPstr = msgCoAPstr + "00000011";
    }
    else if(inputMsg[2] == "DELETE"){
        msgCoAPstr = msgCoAPstr + "00000100";
    }
    else{
        std::cout << "ERROR: Invalid Request" << std::endl;
    }

    //Create Random MEssage ID
    msgCoAPstr += "0100010001001010";

    // Token conversion
    msgCoAPstr += token;

    // OptionDelta conversion
    std::string deltaCode = toCoAPoptionDelta(inputMsg[4]);
    if(deltaCode != "ERROR"){
        msgCoAPstr += deltaCode;
    }
    else{
        std::cout << "ERROR: Invalid Option Delta" << std::endl;
    }

    // Option length
    msgCoAPstr += std::bitset< 4 >( inputMsg[5].length() ).to_string();

    // Option Delta (extended)

    // Option length (extended) 

    // Option Value
    std::vector<char> optionChar(inputMsg[5].begin(), inputMsg[5].end());
    for(char c: optionChar){
        msgCoAPstr += std::bitset<8>(c).to_string();
    }

    // End Header
    msgCoAPstr += "11111111";

    // Body Conversion
    std::vector<char> bodyChar(inputMsg[6].begin(), inputMsg[6].end());
    for(char c: bodyChar){
        msgCoAPstr += std::bitset<8>(c).to_string();
    }

    std::vector<char> byteVector;
    for(int i = 0; i < msgCoAPstr.length()/8; i++){
        byteVector.push_back(binToDec(msgCoAPstr.substr(i*8, 8)));
    }

    return byteVector;

}

std::vector<std::string> fromCoAP(std::vector<char> msg){
    std::string receivedMsg;

    for(char c: msg){
        receivedMsg += std::bitset<8>(c).to_string();
    }

    std::vector<std::string> msgVec;
    
    //Convert Version
    std::string ver = receivedMsg.substr(0, 2);
    receivedMsg.erase(0, 2);

    if(ver == "01"){
        msgVec.push_back("Version 1");
    }
    else{
        msgVec.push_back("Bad_version");
    }

    //Convert Message Type
    std::string msgType = receivedMsg.substr(0, 2);
    receivedMsg.erase(0, 2);

    if(msgType == "00"){
        msgVec.push_back("CON");
    }
    else if(msgType == "01"){
        msgVec.push_back("NON");
    }
    else if(msgType == "10"){
        msgVec.push_back("ACK");
    }
    else if(msgType == "11"){
        msgVec.push_back("RST");
    }
    else{
        msgVec.push_back("Bad_msgType");
    }

    //Convert TKL
    std::string tkl = receivedMsg.substr(0, 4);
    receivedMsg.erase(0, 4);

    int tokenLength = binToDec(tkl);

    //Convert Request CODE
    std::string requestCode = receivedMsg.substr(0, 8);
    receivedMsg.erase(0, 8);

    if(requestCode == "00000000"){
        msgVec.push_back("EMPTY");
    }
    else if(requestCode == "00000001"){
        msgVec.push_back("GET");
    }
    else if(requestCode == "00000010"){
        msgVec.push_back("POST");
    }
    else if(requestCode == "00000011"){
        msgVec.push_back("PUT");
    }
    else if(requestCode == "00000100"){
        msgVec.push_back("DELETE");
    }
    else{
        msgVec.push_back("Bad_requestType");
    }


    //Convert Message ID
    std::string msgID = receivedMsg.substr(0, 16);
    receivedMsg.erase(0, 16);

    msgVec.push_back(msgID);

    //Convert Token
    if(tokenLength != 0){
        std::string token = receivedMsg.substr(0, tokenLength * 8);
        receivedMsg.erase(0, tokenLength * 8);

        //msgVec.push_back(std::to_string(binToDec(token)));
        msgVec.push_back(token);
    }

    //Convert Option Delta
    std::string optionDelta = receivedMsg.substr(0, 4);
    receivedMsg.erase(0, 4);

    std::string delta = fromCoAPoptionDelta(optionDelta);

    if(delta != "ERROR"){
        msgVec.push_back(delta);
    }
    else{
        msgVec.push_back("Bad_OptionDelta");
    }

    //Convert Option Length
    std::string optionLength = receivedMsg.substr(0, 4);
    receivedMsg.erase(0, 4);

    int optionLen = binToDec(optionLength);

    //Convert Option Value
    std::string optionValue = receivedMsg.substr(0, optionLen * 8);
    receivedMsg.erase(0, optionLen * 8);

    msgVec.push_back(binToText(optionValue));

    //Find Header End
    std::string headerEnd = receivedMsg.substr(0, 8);
    receivedMsg.erase(0, 8);

    std::cout << "Header End: " << headerEnd << std::endl;

    //Convert Payload
    msgVec.push_back(binToText(receivedMsg));
    
    return msgVec;
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

std::string binToText(std::string str){
    
    std::stringstream sstream(str);
    std::string output;

    while(sstream.good())
    {
        std::bitset<8> bits;
        sstream >> bits;
        char c = char(bits.to_ulong());
        output += c;
    }

    return output;
}

std::string fromCoAPoptionDelta(std::string delta){
    if(delta == "0001"){
        return "If-Match";
    }
    else if(delta == "0011"){
        return "Uri-Host";
    }
    else if(delta == "0100"){
        return "ETag";
    }
    else if(delta == "0101"){
        return "If-None-Match";
    }
    else if(delta == "0111"){
        return "Uri-Port";
    }
    else if(delta == "1000"){
        return "Location-Path";
    }
    else if(delta == "1011"){
        return "Uri-Path";
    }
    else if(delta == "1100"){
        return "Content-Format";
    }
    else if(delta == "1110"){
        return "Max-Age";
    }
    else if(delta == "1111"){
        return "Uri-Query";
    }
    else{
        return "ERROR";
    }
}

std::string toCoAPoptionDelta(std::string delta){
    if(delta == "If-Match"){
        return "0001";
    }
    else if(delta == "Uri-Host"){
        return "0011";
    }
    else if(delta == "ETag"){
        return "0100";
    }
    else if(delta == "If-None-Match"){
        return "0101";
    }
    else if(delta == "Uri-Port"){
        return "0111";
    }
    else if(delta == "Location-Path"){
        return "1000";
    }
    else if(delta == "Uri-Path"){
        return "1011";
    }
    else if(delta == "Content-Format"){
        return "1100";
    }
    else if(delta == "Max-Age"){
        return "1110";
    }
    else if(delta == "Uri-Query"){
        return "1111";
    }
    else{
        return "ERROR";
    }
}
