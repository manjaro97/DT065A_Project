#include "database.h"
#include <iostream>
#include <fstream>

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
