#include "msgHandler.h"

#include "ConnectHandler.h"
#include "PingHandler.h"
#include "SubscribeHandler.h"


#include <iostream>
#include <fstream>
#include <algorithm>
#include <bitset>

std::vector<std::string> SplitHeader(std::string given_str){
    std::vector<std::string> headerSplit; 

    headerSplit.push_back(given_str.substr(0, 4)); // store the substring

    return headerSplit;
}

std::vector<char> HandleRequest(std::string msgType, std::string given_str){
    std::vector<char> returnMsg;
    int requestNr = std::bitset<16>(msgType).to_ulong();

    switch(requestNr){
        case 0:
            //RESERVED
            break;
        case 1:
            //CONNECT
            std::cout << "---Received Connect Message---" << std::endl;
            returnMsg = DecodeConnect(given_str);
            //After a Network Connection is established by a Client to a Server, the first packet sent from the Client to
            //the Server MUST be a CONNECT packet.
            //A Client can only send the CONNECT packet once over a Network Connection. The Server MUST
            //process a second CONNECT packet sent from a Client as a Protocol Error and close the Network
            //Connection. 
            //The Payload contains one or more encoded fields. They specify a unique Client identifier for the Client, a
            //Will Topic, Will Payload, User Name and Password. All but the Client identifier can be omitted and their
            //presence is determined based on flags in the Variable Header.

            break;
        case 2:
            //CONNACK  (NOT NEEDED)

            //The CONNACK packet is the packet sent by the Server in response to a CONNECT packet received from
            //a Client. The Server MUST send a CONNACK with a 0x00 (Success) Reason Code before sending any
            //Packet other than AUTH. The Server MUST NOT send more than one CONNACK in a Network Connection.
            
            //If the Client does not receive a CONNACK packet from the Server within a reasonable amount of time, the
            //Client SHOULD close the Network Connection. A "reasonable" amount of time depends on the type of
            //application and the communications infrastructure.


            break;
        case 3:
            //PUBLISH

            //A PUBLISH packet is sent from a Client to a Server or from a Server to a Client to transport an Application Message

            break;
        case 4:
            //PUBACK

            //A PUBACK packet is the response to a PUBLISH packet with QoS 1.

            break;
        case 5:
            //PUBREC

            //A PUBREC packet is the response to a PUBLISH packet with QoS 2. It is the second packet of the QoS 2 protocol exchange.

            break;
        case 6:
            //PUBREL

            //A PUBREL packet is the response to a PUBREC packet. It is the third packet of the QoS 2 protocol exchange.


            break;
        case 7:
            //PUBCOMP

            //The PUBCOMP packet is the response to a PUBREL packet. It is the fourth and final packet of the QoS 2 protocol exchange.

            break;
        case 8:

            std::cout << "---Received Subscribe Message---" << std::endl;
            returnMsg = DecodeSubscribe(given_str);
            //SUBSCRIBE

            //The SUBSCRIBE packet is sent from the Client to the Server to create one or more Subscriptions. Each
            //Subscription registers a Client’s interest in one or more Topics. The Server sends PUBLISH packets to
            //the Client to forward Application Messages that were published to Topics that match these Subscriptions.
            //The SUBSCRIBE packet also specifies (for each Subscription) the maximum QoS with which the Server
            //can send Application Messages to the Client.
            break;
        case 9:
            //SUBACK  (NOT NEEDED)

            //A SUBACK packet is sent by the Server to the Client to confirm receipt and processing of a SUBSCRIBE packet.
            //A SUBACK packet contains a list of Reason Codes, that specify the maximum QoS level that was
            //granted or the error which was found for each Subscription that was requested by the SUBSCRIBE.

            break;
        case 10:
            //UNSUBSCRIBE

            //An UNSUBSCRIBE packet is sent by the Client to the Server, to unsubscribe from topics.

            break;
        case 11:
            //UNSUBACK  (NOT NEEDED)

            //The UNSUBACK packet is sent by the Server to the Client to confirm receipt of an UNSUBSCRIBE packet.

            break;
        case 12:

            std::cout << "---Received Ping Request---" << std::endl;
            

            //PINGREQ

            //The PINGREQ packet is sent from a Client to the Server. It can be used to:
            //• Indicate to the Server that the Client is alive in the absence of any other MQTT Control Packets being
            //  sent from the Client to the Server.
            //• Request that the Server responds to confirm that it is alive.
            //• Exercise the network to indicate that the Network Connection is active.

            //This packet is used in Keep Alive processing.

            return DecodePingReq(given_str);
        case 13:
            //PINGRESP  (NOT NEEDED)

            //A PINGRESP Packet is sent by the Server to the Client in response to a PINGREQ packet. It indicates
            //that the Server is alive.

            //This packet is used in Keep Alive processing.

            break;
        case 14:
            //DISCONNECT

            //The DISCONNECT packet is the final MQTT Control Packet sent from the Client or the Server. It
            //indicates the reason why the Network Connection is being closed. The Client or Server MAY send a
            //DISCONNECT packet before closing the Network Connection. If the Network Connection is closed
            //without the Client first sending a DISCONNECT packet with Reason Code 0x00 (Normal disconnection)
            //and the Connection has a Will Message, the Will Message is published. 

            //A Server MUST NOT send a DISCONNECT until after it has sent a CONNACK with Reason Code of less than 0x80

            break;
        case 15:
            //RESERVED
            break;
    }

    return returnMsg;
}

/*
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

*/