#include "database.h"
#include <iostream>
#include <fstream>
#include <algorithm>

DB::DB()
{

}

void DB::PublishToDB(std::string topic, std::string message, int retain){
    
    //Add Data to Queued List to send out to Subscribers
    std::pair<std::string,std::string> msgPair(topic, message);
    if(queuedTopics.find(topic) != queuedTopics.end()){
        queuedTopics[topic] = message;
    }
    else{
        queuedTopics.insert(msgPair);
    }

    //Add Topic to Subscription List
    if(listOfSubscriptions.find(topic) == listOfSubscriptions.end()){
        std::vector<SOCKET> socketList;
        std::pair<std::string, std::vector<SOCKET>> subPair(topic, socketList);
        listOfSubscriptions.insert(subPair);
    }

    //Add data of Topic to database
    if(listOfTopicData.find(topic) != listOfTopicData.end()){
        listOfTopicData[topic] = message;
    }
    else{
        listOfTopicData.insert(msgPair);
    }

    //Set if Topic should be Retained
    std::pair<std::string,int> msgRetain(topic, retain);
    if(isTopicRetained.find(topic) != isTopicRetained.end()){
        isTopicRetained[topic] = retain;
    }
    else{
        isTopicRetained.insert(msgRetain);
    }
}

void DB::AddSubscription(std::string topic, SOCKET clientSOCKET){

    if(listOfSubscriptions.find(topic) != listOfSubscriptions.end()){
        std::vector<SOCKET> socketList = listOfSubscriptions[topic];
        socketList.push_back(clientSOCKET);
        listOfSubscriptions[topic] = socketList;
        std::cout << "Client: " << clientSOCKET << " Added to Topic: " << topic << std::endl;
    }
    else{
        std::cout << "Topic does not exist, Subscription Failed" << std::endl;
    }

}

void DB::RemoveSubscription(std::string topic, SOCKET clientSOCKET){

    if(listOfSubscriptions.find(topic) != listOfSubscriptions.end()){
        std::vector<SOCKET> socketList = listOfSubscriptions[topic];

        if(std::find(socketList.begin(), socketList.end(), clientSOCKET) != socketList.end()){
            socketList.erase(std::remove(socketList.begin(), socketList.end(), clientSOCKET), socketList.end());
            listOfSubscriptions[topic] = socketList;
            std::cout << "Client: " << clientSOCKET << " Subscription Removed from Topic: " << topic << std::endl;
        }
        else{
            std::cout << "Subscription does not exist within Topic" << std::endl;
        }
    }
    else{
        std::cout << "Topic does not exist, Failed to Unsubscribe" << std::endl;
    }
}

void DB::DisconnectEraseAll(SOCKET clientSOCKET){
    for(std::pair<std::string, std::vector<SOCKET>> p: listOfSubscriptions){
        RemoveSubscription(p.first, clientSOCKET);
    }
}

std::map<std::string, std::string> DB::GetRetained(){
    std::map<std::string, std::string> m;

    for(std::pair<std::string, int> p : isTopicRetained){
        if(p.second == 1){
            std::cout << p.first << " is Retained" << std::endl;
            if(listOfTopicData.find(p.first) != listOfTopicData.end()){
                std::string topic = p.first;
                std::string data = listOfTopicData[p.first];
                std::pair<std::string, std::string> p(topic, data);
                
                m.insert(p);
            }
        }
    }
    return m;
}

std::map<std::string, std::vector<SOCKET>> DB::GetListOfSubscriptions(){
    return listOfSubscriptions;
}

std::map<std::string, std::string> DB::GetNewData(){
    return queuedTopics;
}

void DB::EraseDataFromQueue(){
    queuedTopics.clear();
}

/*
std::vector<std::vector<std::string>> readDatabase(){
    
    std::vector<std::vector<std::string>> storedData;
    int databaseSize = 0;
    std::string delimiter = ": ";
    std::string line;
    std::ifstream myfile ("topics.txt");
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

    std::ofstream myfile1 ("topics.txt");
    myfile1.open("database.txt", std::ofstream::out | std::ofstream::trunc);
    myfile1.close();
    std::ofstream myfile2 ("topics.txt");
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
