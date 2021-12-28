#include "data.h"
#include <iostream>
#include <fstream>
#include <algorithm>

DATA::DATA()
{

}

void DATA::dataPublish(std::string topic, std::string message){
    
    std::pair<std::string,std::string> msgPair(topic, message);

    //Add data of Topic to database
    if(listOfTopicData.find(topic) != listOfTopicData.end()){
        listOfTopicData[topic] = message;
    }
    else{
        listOfTopicData.insert(msgPair);
    }
    return;
}

std::map<std::string, std::string> DATA::GetData(){
    return listOfTopicData;
}
