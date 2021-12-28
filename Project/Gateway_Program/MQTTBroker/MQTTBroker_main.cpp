#include <iostream>
#include <string>
#include <vector>

#include "MQTTBroker_TCPlistener.h"
#include "MQTTBroker_msgHandler.h"
#include "MQTTBroker_Header.h"

// cd "c:\Users\j_c_k\Desktop\DT065A_Project\DT065A_Project\Project\Gateway_Program\MQTTBroker\" ; if ($?) { g++ *.cpp -o main -lws2_32 } ; if ($?) { .\main }

DB databaseObj;
void runMQTTBroker();

/*int main(){

	TcpConnection connection;
	connection.Start();

	return 0;
}*/

void runMQTTBroker(){
	
	TcpConnection connection;
	connection.Start();

	return;
}
