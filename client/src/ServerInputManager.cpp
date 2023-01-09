
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
        connectionHandler.getLine(serverInput);
        //Send to protocol to manage the input
        Frame frame = toFrameRecieve(serverInput);
        connectionHandler.protocol.receiveProcess(frame);
        std::cout << "frame recived from server\n" + frame.toString() << std::endl;
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
            body += line;
        }
    }
    return Frame(commandLine, headers, body);
};