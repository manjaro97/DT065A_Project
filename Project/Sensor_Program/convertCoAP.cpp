#include <iostream>
#include <bitset>
#include <sstream>
#include "convertCoAP.h"

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

    if(inputMsg[6].size() != 0){

        // Body Conversion
        std::vector<char> bodyChar(inputMsg[6].begin(), inputMsg[6].end());
        for(char c: bodyChar){
            msgCoAPstr += std::bitset<8>(c).to_string();
        }
    }

    std::vector<char> byteVector;
    for(int i = 0; i < msgCoAPstr.length()/8; i++){
        byteVector.push_back(std::bitset<8>(msgCoAPstr.substr(i*8, 8)).to_ullong());
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

    int tokenLength = std::bitset<8>(tkl).to_ullong();

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

    //Find Header End
    std::string headerEnd = receivedMsg.substr(0, 8);

    //Preset Option Delta
    std::string optionDelta = "0000";

    //Loop Options
    while(headerEnd != "11111111"){
        //Convert Option Delta

        optionDelta = std::bitset<4>(std::bitset<8>(optionDelta).to_ullong() + std::bitset<8>(receivedMsg.substr(0, 4)).to_ullong()).to_string();

        receivedMsg.erase(0, 4);
        std::string delta;
        int extendedOptionDelta = 0;

        if(optionDelta == "1101"){
            extendedOptionDelta = 1;
            delta = "Option Delta extended 1 Byte";
        }
        else if(optionDelta == "1110"){
            extendedOptionDelta = 2;
            delta = "Option Delta extended 2 Bytes";
        }
        else if(optionDelta == "1111"){
            delta = "MESSAGE FORMAT ERROR";
        }
        else{
            delta = fromCoAPoptionDelta(optionDelta);
        }

        if(delta != "ERROR"){
            msgVec.push_back(delta);
        }
        else{
            msgVec.push_back("Bad_OptionDelta");
        }

        //Convert Option Length
        std::string optionLength = receivedMsg.substr(0, 4);
        receivedMsg.erase(0, 4);

        int optionLen;
        std::string optionLenMsg;
        int extendedOptionLength = 0;

        if(optionDelta == "1101"){
            extendedOptionLength = 1;
            optionLenMsg = "Option Delta extended 1 Byte";
            msgVec.push_back("optionLenMsg");
        }
        else if(optionDelta == "1110"){
            extendedOptionLength = 2;
            optionLenMsg = "Option Delta extended 2 Bytes";
            msgVec.push_back("optionLenMsg");
        }
        else if(optionDelta == "1111"){
            optionLenMsg = "MESSAGE FORMAT ERROR";
            msgVec.push_back("optionLenMsg");
        }
        else{
            optionLen = std::bitset<8>(optionLength).to_ullong();
        }

        //Convert Option Delta Extended

        if(extendedOptionDelta == 1){
            std::string optionDeltaExt = receivedMsg.substr(0, 8);
            receivedMsg.erase(0, 8);

            if(delta != "ERROR"){
                msgVec.push_back(delta);
            }
            else{
                msgVec.push_back("Bad_OptionDelta");
            }

        }
        else if(extendedOptionDelta == 2){
            std::string optionDeltaExt = receivedMsg.substr(0, 16);
            receivedMsg.erase(0, 16);
        }

        //Convert Option Length Extended
        if(extendedOptionLength == 1){
            std::string optionLengthExt = receivedMsg.substr(0, 8);
            receivedMsg.erase(0, 8);

            optionLen = std::bitset<8>(optionLengthExt).to_ullong();
        }
        else if(extendedOptionLength == 2){
            std::string optionLengthExt = receivedMsg.substr(0, 16);
            receivedMsg.erase(0, 16);

            optionLen = std::bitset<16>(optionLengthExt).to_ullong();
        }

        //Convert Option Value
        std::string optionValue = "";
        for (int i = 0; i < optionLen; i++)
        {
            optionValue += char(std::bitset<8>(receivedMsg.substr(0, 8)).to_ulong());
            receivedMsg.erase(0, 8);
        }

        msgVec.push_back(optionValue);

        //Find Header End
        headerEnd = receivedMsg.substr(0, 8);
    }

    msgVec.push_back("Header ending at: " + receivedMsg.substr(0, 8));
    receivedMsg.erase(0, 8);

    //Convert Payload
    std::string payload = "";
    int payloadLen = receivedMsg.length()/8;
    for (int i = 0; i < (payloadLen); i++)
    {
        payload += char(std::bitset<8>(receivedMsg.substr(0, 8)).to_ulong());
        receivedMsg.erase(0, 8);
    }
    msgVec.push_back(payload);
    
    return msgVec;
}

std::string fromCoAPoptionDelta(std::string delta){
    int deltaNum = std::bitset<4>(delta).to_ulong();
    switch(deltaNum){
        case 1:
            return "If-Match";
        case 3:
            return "Uri-Host";
        case 4:
            return "ETag";
        case 5:
            return "If-None-Match";
        case 7:
            return "Uri-Port";
        case 8:
            return "Location-Path";
        case 11:
            return "Uri-Path";
        case 12:
            return "Content-Format";
        case 14:
            return "Max-Age";
        case 15:
            return "Uri-Query";
        default:
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
