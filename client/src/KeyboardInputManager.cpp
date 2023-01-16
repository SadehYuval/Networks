#include "KeyboardInputManager.h"
#include "Frame.h"
#include "event.h"
#include <string>
#include <map>

using std::unordered_map;
using std::string;
using std::pair;
using std::map;



KeyboardInputManager::KeyboardInputManager(ConnectionHandler &connectionHandler): connectionHandler(connectionHandler),receiptNum(0){};

void KeyboardInputManager::run(){
    string keyboardInput = "";
    while(!connectionHandler.protocol.should_terminate){
        //Read from keyboard and insert to keyboardInput
        getline(std::cin, keyboardInput);
        //send to protocol to handle the input
        if(!connectionHandler.protocol.should_terminate){
            list<Frame> frameList;
            toFrameSend(keyboardInput,frameList);
            for(auto& frame: frameList){
                if(frame.getCommandLine().compare("SUMMARY") == 0){
                    connectionHandler.protocol.summaryProcess(frame);
                }
                else if(frame.getCommandLine().compare("CONNECT") == 0 &&  connectionHandler.protocol.connected){
                    std::cout << "already logged in, logout before trying to login" << std::endl;
                }
                else{
                    string output = frame.toString();
                    connectionHandler.sendFrameAscii(output, '\0');
                }
            }
        }
    }
};

void KeyboardInputManager::toFrameSend(string &convert, list<Frame> & frameList){
    receiptNum++;
    string commandLine = "";
    map<string, string> headers;
    string body = "";
    vector<string> lines = connectionHandler.split(convert, ' ');
    string type = lines[0];
    //frames to send
    //login - CONNECT
    if(type.compare("login") == 0){
        commandLine = "CONNECT";
        //Decide what to do with illegal input - temporary solution:
        if(lines.size() != 4){
            commandLine = "CONNECT INVALID";
            pair<string, string> header ("error","");
            headers.insert(header);
        }
        else{
            connectionHandler.protocol.setUserName(lines[2]);
            pair<string, string> version ("accept-version", "1.2");
            pair<string, string> host ("host", "stomp.cs.bgu.ac.il");
            pair<string, string> login ("login", lines[2]);
            pair<string, string> passcode ("passcode", lines[3]);
            headers.insert(version);
            headers.insert(host);
            headers.insert(login);
            headers.insert(passcode);
        }
        Frame frame = Frame(commandLine, headers, body);
        frameList.push_back(frame);
    }
    //logout - DISCONNECT
    else if(type.compare("logout") == 0){
        commandLine = "DISCONNECT";
        if(lines.size() != 1){
            commandLine = "DISCONNECT INVALID";
            pair<string, string> header ("error","");
            headers.insert(header);
        }
        else{
            string receipt_num = std::to_string(receiptNum);
            pair<string, string> header ("receipt",  receipt_num);
            headers.insert(header);
            connectionHandler.protocol.setLogoutReceipt(receiptNum);
        }
        Frame frame = Frame(commandLine, headers, body);
        frameList.push_back(frame);
    }
    //join - SUBSCRIBE
    else if(type.compare("join") == 0){
        commandLine = "SUBSCRIBE";
        if(lines.size() != 2){
            commandLine = "SUBSCRIB INVALID";
            pair<string, string> header ("error","");
            headers.insert(header);
        }
        else{
            pair<string, string> destination ("destination", "/" + lines[1]);
            connectionHandler.protocol.addSubscription(lines[1]);
            int subID = connectionHandler.protocol.getSubId(lines[1]);
            string sub_id = std::to_string(subID);
            pair<string, string> id ("id", sub_id);
            string receipt_num = std::to_string(receiptNum);
            pair<string, string> receipt ("receipt",  receipt_num);
            headers.insert(destination);
            headers.insert(id);
            headers.insert(receipt);
        }
        Frame frame = Frame(commandLine, headers, body);
        frameList.push_back(frame);
    }
    //exit - UNSUBSCRIBE
    else if(type.compare("exit") == 0){
        commandLine = "UNSUBSCRIBE";
        if(lines.size() != 2){
            commandLine = "UNSUBSCRIB INVALID";
            pair<string, string> header ("error","");
            headers.insert(header);
        }
        else{
            int subID = connectionHandler.protocol.getSubId(lines[1]);
            connectionHandler.protocol.removeSubscription(lines[1]);
            string sub_id = std::to_string(subID);
            pair<string, string> id ("id", sub_id);
            string receipt_num = std::to_string(receiptNum);
            pair<string, string> receipt ("receipt",  receipt_num);
            headers.insert(receipt);
            headers.insert(id);
        }
        Frame frame = Frame(commandLine, headers, body);
        frameList.push_back(frame);
    }


    //report - SEND
    else if(type.compare("report") == 0){
        commandLine = "SEND";
        if(lines.size() != 2){
            commandLine = "SEND INVALID";
            pair<string, string> header ("error","");
            headers.insert(header);
            Frame frame = Frame(commandLine, headers, body);
            frameList.push_back(frame);
        }
        else{
            names_and_events toReport = parseEventsFile(lines[1]);
            string game = "/" + toReport.team_a_name + "_" + toReport.team_b_name;
            pair<string, string> gameName ("destination", game);
            headers.insert(gameName);
            for(auto& event:toReport.events){
                body =  "user: " + connectionHandler.protocol.getUserName() + "\n";
                body = body + toStringFile(event);
                Frame frame = Frame(commandLine, headers, body);
                frameList.push_back(frame);
            }
        }   
    }


    //summary
    else if(type.compare("summary") == 0){
        commandLine = "SUMMARY";
        if(lines.size() != 4){
            commandLine = "SUMMARY INVALID";
            pair<string, string> header ("error","");
            headers.insert(header);
        }
        else{
            pair<string, string> game_name ("game", lines[1]);
            pair<string, string> user ("user", lines[2]);
            pair<string, string> file ("file", lines[3]);
            headers.insert(game_name);
            headers.insert(user);
            headers.insert(file);
        }
        Frame frame = Frame(commandLine, headers, body);
        frameList.push_back(frame);
    }
    //Invalid command
    else{
        commandLine = "Invalid command line";
        pair<string, string> header ("error","");
        headers.insert(header);
        Frame frame = Frame(commandLine, headers, body);
        frameList.push_back(frame);
    }
    
};

string KeyboardInputManager::toStringFile(Event &event){
    std::stringstream body;
    string general_game_updates = "general game updates:\n";
    string team_a_updates = "team a updates:\n";
    string team_b_updates= "team b updates:\n";
    for(const auto& update : event.get_game_updates()){
        general_game_updates += "   " + update.first + ": " + update.second + '\n';
    }
    for(const auto& update : event.get_team_a_updates()){
        team_a_updates += "   " + update.first + ": " + update.second + '\n';
    }
    for(const auto& update : event.get_team_b_updates()){
        team_b_updates += "   " + update.first + ": " + update.second + '\n';
    }
    body << "team a: " + event.get_team_a_name() + '\n' <<
    "team b: " + event.get_team_b_name() + '\n' <<
    "event name: " + event.get_name() + '\n' <<
    "time: " + std::to_string(event.get_time()) + '\n' <<
    general_game_updates <<
    team_a_updates <<
    team_b_updates <<
    "description:\n" + event.get_discription();

    return body.str();
    
};