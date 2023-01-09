#pragma once
#include "ConnectionHandler.h"


class KeyboardInputManager
{
private:
    ConnectionHandler &connectionHandler;
    Frame toFrameSend(string &convert);
    int receiptNum;
    string toStringFile(Event &event);
public:
    KeyboardInputManager(ConnectionHandler &connectionHandler);
    void run();
  
};

