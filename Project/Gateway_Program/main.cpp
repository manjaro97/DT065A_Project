#include <iostream>
#include "UDPClient_main.h"
#include "MQTTClient_main.h"
#include "MQTTBroker_main.h"
#include <windows.h>
#include <process.h>

// cd "C:\Users\Manjaro\Desktop\DT065A_Project\DT065A_Project\Project\Gateway_Program\" ; if ($?) { g++ main.cpp sendMessage.cpp sendMessage.h convertCoAP.cpp convertCoAP.h -o main -lws2_32} ; if ($?) { .\main }
// cd "C:\Users\j_c_k\Desktop\DT065A_Project\DT065A_Project\Project\Gateway_Program\" ; if ($?) { g++ -I ./UDPandMQTTClient -I ./MQTTBroker main.cpp UDPandMQTTClient/*.cpp MQTTBroker/*.cpp -o main -lws2_32} ; if ($?) { .\main }


unsigned int __stdcall ThreadUDPClient(void* data);
unsigned int __stdcall ThreadMQTTBroker(void* data);
unsigned int __stdcall ThreadMQTTClient(void* data);

int main(){

    _beginthreadex(0, 0, ThreadUDPClient, 0, 0, 0);
    _beginthreadex(0, 0, ThreadMQTTBroker, 0, 0, 0);
    _beginthreadex(0, 0, ThreadMQTTClient, 0, 0, 0);

	while(true){


    }

	return 0;
}

unsigned int __stdcall ThreadUDPClient(void* data)
{
    runUDPClient();

    return 0;
}

unsigned int __stdcall ThreadMQTTBroker(void* data)
{
    runMQTTBroker();

    return 0;
}

unsigned int __stdcall ThreadMQTTClient(void* data)
{
    runMQTTClient();

    return 0;
}