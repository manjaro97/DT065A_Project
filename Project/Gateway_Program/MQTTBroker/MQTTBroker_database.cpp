#include "MQTTBroker_database.h"
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

    //For retained messages
    std::pair<SOCKET, std::string> queuedSubscriber(clientSOCKET, topic);
    queuedSubscription.insert(queuedSubscriber);
    
    std::cout << "Client: " << clientSOCKET << " Added to retain queue of topic: " << topic << std::endl;
    for(std::pair<SOCKET, std::string> p : queuedSubscription){
        std::cout << "Client: " << clientSOCKET << " Added to retain queue of topic: " << topic << std::endl;
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

std::map<std::string, std::vector<SOCKET>> DB::GetListOfSubscriptions(){
    return listOfSubscriptions;
}

std::map<std::string, std::string> DB::GetNewData(){
    return queuedTopics;
}

void DB::EraseDataFromQueue(){
    queuedTopics.clear();
}

std::pair<std::string, std::string> DB::GetRetainedMsg(SOCKET clientSOCKET){
    std::string topic = queuedSubscription[clientSOCKET];
    queuedSubscription.clear();

    if(isTopicRetained[topic] == 1){
        std::cout << topic << " is Retained" << std::endl;

        std::pair<std::string, std::string> data(topic, listOfTopicData[topic]);
        return data;
    }
    else{
        std::pair<std::string, std::string> emptyReturn(topic, "");
        return emptyReturn;
    }
}
