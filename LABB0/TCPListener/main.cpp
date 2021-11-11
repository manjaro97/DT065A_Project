#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include<algorithm>

#include "TCPlistener.h"

// cd "c:\Users\j_c_k\Desktop\DT065A_Project\DT065A_Project\TCPListener\" ; if ($?) { g++ main.cpp TcpListener.cpp TcpListener.h database.txt -o main -lws2_32 } ; if ($?) { .\main }

void Listener_MessageReceived(CTcpListener* listener, int client, std::string msg);
std::vector<std::string> SplitHeaderBody(std::string given_str, std::string delimiter);
std::string HandleRequest(std::string request, std::string path, std::string body);
std::vector<std::vector<std::string>> readDatabase();
void updateDatabase(std::vector<std::vector<std::string>> storedData);

int main(){

    CTcpListener server("127.0.0.1", 54010, Listener_MessageReceived);
    
    if(server.init()){
        server.Run();
    }
}

void Listener_MessageReceived(CTcpListener* listener, int client, std::string msg){

    std::string given_str = msg; // GET /Address/ HTTP/1.1/\r\nHeader Stuff\r\nDoc-TYPE = lol\r\n\r\nBody - Testing hoping this shit works sometime
    std::string responseMsg = "*Basic* HTTP Status 200 (OK)";

    std::vector<std::string> msgSplit = SplitHeaderBody(given_str, "\\r\\n\\r\\n");
    if(msgSplit.size() == 2){
        std::cout << "Header: "<< msgSplit[0] << std::endl;
        std::cout << "Body: " << msgSplit[1] << std::endl;

        std::vector<std::string> headerSplit = SplitHeaderBody(msgSplit[0], "\\r\\n");

        if(headerSplit.size() != 0){
            std::vector<std::string> requestSplit = SplitHeaderBody(headerSplit[0], " ");
            
            if(requestSplit.size() == 3){
                // Not needed
                std::string request = requestSplit[0];
                std::string path = requestSplit[1];
                std::string requestType = requestSplit[2];
                std::string body = msgSplit[1];

                std::cout << "Before HandleRequest" << std::endl;
                responseMsg = HandleRequest(request, path, body);
            }
            else{
                listener->Send(client, "400 - Error: Request Splitting Failed, Faulty request");
                return;
            }
        }
        else{
            listener->Send(client, "400 - Error: Header Splitting Failed, Faulty request");
            return;
        }
    }
    else{
        listener->Send(client, "400 - Error: Message Splitting Failed, Faulty request");
        return;
    }    

    listener->Send(client, responseMsg);
    std::cout << "Message: " << responseMsg << std::endl;
    return;
}

std::vector<std::string> SplitHeaderBody(std::string given_str, std::string delimiter){
    
    std::vector<std::string> msgSplit; 
    size_t pos = 0;

    /*if(given_str.find (delimiter) == std::string::npos){
        return msgSplit;
    }*/

    while (( pos = given_str.find (delimiter)) != std::string::npos)  
    {  
        msgSplit.push_back(given_str.substr(0, pos)); // store the substring  
        given_str.erase(0, pos + delimiter.length()); // erase previous substring   
    }  
    msgSplit.push_back(given_str);

    return msgSplit;
}

std::string HandleRequest(std::string request, std::string path, std::string body){

    std::vector<std::vector<std::string>> storedData = readDatabase();

    if(request == "POST"){
        // Should be used to initially create a sensor entity so that sensor values can be added later. 
        // Return 201 if successfully created and 409 if the sensor already exists.
        
        for(int i = 0; i < storedData.size(); i++){
            if(storedData[i][0] == body){
                return "409 - Error: Sensor Already Exists";
            }
        }
        
        storedData.push_back(std::vector<std::string>());
        storedData[storedData.size()-1].push_back(body);
        storedData[storedData.size()-1].push_back(": ");
        storedData[storedData.size()-1].push_back("");

        updateDatabase(storedData);
        return "200 - Success, stored sensor";
    }
    else if(request == "PUT"){
        // Should be used to persistently save a sensor value on the REST server. 
        // Return 200 if updated and 404 if not found.

        std::string pathID = path;
        pathID.erase(remove(pathID.begin(), pathID.end(), '/'), pathID.end()); //remove A from string
        
        bool pathIDExists = false;
        for(int i = 0; i < storedData.size(); i++){
            if(storedData[i][0] == pathID){
                pathIDExists = true;
                storedData[i][2] = body;
            }
        }
        if(pathIDExists == false){
            return "409 - Error: Sensor does not exist";
        }
        
        updateDatabase(storedData);
        return "200 - Success, updated data";

    }
    else if(request == "GET"){
        // Should be used to retrieve a sensor value from the REST server. 
        // Return 200 with the value or 404 if not found.
        std::string pathID = path;
        pathID.erase(remove(pathID.begin(), pathID.end(), '/'), pathID.end()); //remove A from string
        
        bool pathIDExists = false;
        for(int i = 0; i < storedData.size(); i++){
            if(storedData[i][0] == pathID){
                pathIDExists = true;
                return "200 - Success, value: " + storedData[i][2];
            }
        }
        if(pathIDExists == false){
            return "409 - Error: Sensor does not exist";
        }
        return "400 - Error: Something is wroong in GET";
    }
    else if(request == "DELETE"){
        // Should be used to remove a sensor entity and its sensor value from the REST server. 
        // Return 404 if not found or 200 if successfully deleted. 
    }
    else{return "400 - Error: Bad Request Variable";}

    return "end of function";
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