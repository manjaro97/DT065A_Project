#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <string>
#include <map>
#include <WS2tcpip.h>


/*std::vector<std::vector<std::string>> readDatabase();
void updateDatabase(std::vector<std::vector<std::string>> storedData);*/

class DB {       
  public:   
    DB();          
    void PublishToDB(std::string topic, std::string message, int retain);
    void AddSubscription(std::string topic, SOCKET clientSOCKET);
    void RemoveSubscription(std::string topic, SOCKET clientSOCKET); 
    void DisconnectEraseAll(SOCKET clientSOCKET);
    std::map<std::string, std::string> GetRetained();
    std::map<std::string, std::vector<SOCKET>> GetListOfSubscriptions();
    std::map<std::string, std::string> GetNewData(); 
    void EraseDataFromQueue();
    

  private:  
    std::map<std::string, std::vector<SOCKET>> listOfSubscriptions; //Clients Subscribing to Topics
    std::map<std::string, std::string> listOfTopicData; //List of last stored data to Topics
    std::map<std::string, int> isTopicRetained; //List of last stored data to Topics
    std::map<std::string, std::string> queuedTopics; //List of data to be sent out
};

#endif

