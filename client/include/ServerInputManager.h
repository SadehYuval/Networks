#pragma once
#include "ConnectionHandler.h"

class ServerInputManager
{
private:
    ConnectionHandler &connectionHandler;
    
public:
    ServerInputManager(ConnectionHandler &connectionHandler);
    Frame toFrameRecieve(string &convert);
    void run();
};