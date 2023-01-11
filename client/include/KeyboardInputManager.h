#pragma once
#include "ConnectionHandler.h"


class KeyboardInputManager
{
private:
    ConnectionHandler &connectionHandler;
    void toFrameSend(string &convert,list<Frame>& frameList);
    int receiptNum;
    string toStringFile(Event &event);
public:
    KeyboardInputManager(ConnectionHandler &connectionHandler);
    void run();
  
};

