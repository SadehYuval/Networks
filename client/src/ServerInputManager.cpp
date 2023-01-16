
#include "ServerInputManager.h"
#include "Frame.h"
#include <map>
#include <string>

using std::map;
using std::string;
using std::pair;




ServerInputManager::ServerInputManager(ConnectionHandler &connectionHandler): connectionHandler(connectionHandler){};

void ServerInputManager::run(){
    
    while(!connectionHandler.protocol.should_terminate){
        string serverInput = "";
        //Read from the socket and insert into serverInput
        connectionHandler.getFrameAscii(serverInput,'\0');
        //Send to protocol to manage the input
        Frame frame = toFrameRecieve(serverInput);
        connectionHandler.protocol.receiveProcess(frame);
        
    }
};

Frame ServerInputManager::toFrameRecieve(string &convert){
    string commandLine ="";
    map<string, string> headers;
    string body = "";
    vector<string> lines = connectionHandler.split(convert, '\n');
    commandLine = lines[0];
    bool inHeaders = true;
    int length = lines.size();
    for(int i = 1; i<length; i++){
        string line = lines[i];
        if(inHeaders && line.size() == 0) inHeaders = false;
        else if(inHeaders){
            int dotIndex = line.find(':');
            string key = line.substr(0, dotIndex);
            string value = line.substr(dotIndex +1);
            pair<string, string> tempPair (key, value);
            headers.insert(tempPair);
        }
        else{
            body = body + line + '\n';
        }
    }
    return Frame(commandLine, headers, body);
};