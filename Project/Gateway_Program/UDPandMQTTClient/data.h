#ifndef DATA_H
#define DATA_H

#include <vector>
#include <string>
#include <map>
#include <WS2tcpip.h>


/*std::vector<std::vector<std::string>> readDatabase();
void updateDatabase(std::vector<std::vector<std::string>> storedData);*/

class DATA {       
  public:   
    DATA();          
    void dataPublish(std::string topic, std::string message);
    std::map<std::string, std::string> GetData(); 

  private:  
    std::map<std::string, std::string> listOfTopicData; //List of last stored data to Topics
};

#endif

