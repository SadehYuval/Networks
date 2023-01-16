#include "../include/StompClient.h"
#include "../include/ConnectionHandler.h"
#include "../include/ServerInputManager.h"
#include "../include/KeyboardInputManager.h"
#include "Frame.h"
#include <thread>
#include <boost/lexical_cast.hpp>
#include <vector>


using std::string;
using std::pair;
using std::map;
using std::vector;

//Termination condition

vector<string> split(string &input,char delimiter){
    vector<string> lines;
    std::stringstream ss(input);
    string line;
    while (std::getline(ss, line, delimiter)) {
        lines.push_back(line);
    }
    return lines;
}

Frame toLoginFrame(vector<string> &arguments){
    string commandLine = "CONNECT";
    map<string,string> headers;
    string body = "";
    if(arguments.size() != 4 || arguments[0].compare("login") != 0){
        pair<string,string> error ("error","");
        headers.insert(error);
    }
    else{
        pair<string, string> version ("accept-version", "1.2");
        pair<string, string> host ("host", "stomp.cs.bgu.ac.il");
        pair<string, string> login ("login", arguments[2]);
        pair<string, string> passcode ("passcode", arguments[3]);
        headers.insert(version);
        headers.insert(host);
        headers.insert(login);
        headers.insert(passcode);
    }
    return Frame(commandLine,headers,body);
}



int main () {
    while(1){
        std::cout << "Please login with valid arguments: login {host:port} {user name} {password} " << std::endl;
        string keyboardInput = "";
        getline(std::cin, keyboardInput);
        vector<string> arguments = split(keyboardInput,' ');
        Frame loginFrame = toLoginFrame(arguments);
        string arg1 = "";
        if(arguments.size() >= 2){
            arg1 = arguments[1];
        }
        vector<string> hostPort = split(arg1,':');
        if(hostPort.size() == 2 && arguments[0].compare("login") == 0){
            short port = boost::lexical_cast<short>(hostPort[1]);
            string host = hostPort[0];
            ConnectionHandler connectionHandler(host, port);
            if (!connectionHandler.connect()) {
                std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
                return 1;
            }
            connectionHandler.protocol.setUserName(arguments[2]);
            KeyboardInputManager readFromKeyboard(connectionHandler);
	        ServerInputManager readFromServer(connectionHandler);

            string login = loginFrame.toString();
            connectionHandler.sendFrameAscii(login, '\0');
            //waiting for response from the server
            string serverInput = "";
            connectionHandler.getFrameAscii(serverInput,'\0');
            Frame serverFrame = readFromServer.toFrameRecieve(serverInput);
            connectionHandler.protocol.receiveProcess(serverFrame);
            
            
            //std::thread thread_readFromKeyboard(&KeyboardInputManager::run, &readFromKeyboard);
		    std::thread thread_readFromServer(&ServerInputManager::run, &readFromServer);
            readFromKeyboard.run();
            //thread_readFromKeyboard.join();
		    thread_readFromServer.join();

            connectionHandler.close();
            std::cout << "Socket has been closed and the connection was terminated" << std::endl;
        } 
        else
            std::cout << "Please attemp to login with valid arguments first" << std::endl;
    }
	return 0;
}


