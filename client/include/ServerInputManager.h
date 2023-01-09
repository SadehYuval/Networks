#pragma once
#include "ConnectionHandler.h"

class ServerInputManager
{
private:
    ConnectionHandler &connectionHandler;
    Frame toFrameRecieve(string &convert);
public:
    ServerInputManager(ConnectionHandler &connectionHandler);
    void run();
};