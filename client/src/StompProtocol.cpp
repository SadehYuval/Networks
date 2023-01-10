
#include "StompProtocol.h"
#include "ConnectionHandler.h"
#include "StompClient.h"
#include "Frame.h"
#include "event.h"
#include <map>
#include <unordered_map>
#include <map>
#include <string>
#include <vector>
#include <fstream>

using std::vector;
using std::string;
using std::stringstream;
using std::unordered_map;
using std::pair;
using std::map;


StompProtocol::StompProtocol(): gamesToSubId(), userName(""), subsId(0), reportsMap(),should_terminate(false),logoutReceipt(-1), connected(false){};

StompProtocol::~StompProtocol(){
    
}

void StompProtocol::summaryProcess(Frame &frame){
    //TODO
    string userName = frame.getHeaders().at("user");
    string game = frame.getHeaders().at("game");
    string txtFile = frame.getHeaders().at("file");
    try{
        gamesToSubId.at(game);
        try{
            pair<string,string> gameToUser(game,userName); 
            list<Frame> updates = reportsMap.at(gameToUser);
            map<string,string> general_game_updates;
            map<string,string> team_a_updates;
            map<string,string> team_b_updates;
            map<string,string> game_event_before_half_time;
            map<string,string> game_event_after_half_time;
            //TODO

        }catch(const std::out_of_range& noReports){
            //user not reported on this game
            //return an empty file
            std::ofstream file(txtFile);

            std::cout << "user not reported on this game" << std::endl;
        }

    }catch(const std::out_of_range& unSubscribed){
        //user not subscribed to this game
        should_terminate = true;
        std::cout << "user not subscribed to this game" << std::endl;
    }
};

void StompProtocol::receiveProcess(Frame &frame){
    string commandLine = frame.getCommandLine();
     //CONNECTED
    if(commandLine.compare("CONNECTED") == 0){
        connected = true;
    }
    //
    //ERROR -> should_terminate=true
    
    if(commandLine.compare("ERROR") == 0){
        connected = false;
        should_terminate = true;
    }
   
    //MESSAGE
    if(commandLine.compare("MESSAGE") == 0){
        string userName;
        std::stringstream ss (frame.getBody());
        //TODO
        //user name incurrect!!!"username: mani" for example and \n not detected
        std::getline(ss, userName, '\n');
        std::cout << "user that send the mesage: "+ userName << std::endl;
        string game = frame.getHeaders().at("destination");
        pair<string,string> temp (game, userName);
        list<Frame>* updates;
        try{
            updates = &reportsMap.at(temp);
            updates->push_back(frame);

        }catch(const std::out_of_range& oor){
            list<Frame> newUpdates;
            newUpdates.push_back(frame);
            pair<pair<string,string>,list<Frame>> insert (temp, newUpdates);
            reportsMap.insert(insert);   
        }
    }
    //RECEIPT
    if(commandLine.compare("RECEIPT") == 0){
        string reseiptNum = frame.getHeaders().at("receipt-id");
        if(reseiptNum.compare(std::to_string(logoutReceipt)) == 0){
            connected = false;
            should_terminate = true;
        } 
    }
    
};

void  StompProtocol::addSubscription(string game){
    std::pair<string,int> new_sub (game,subsId);
    gamesToSubId.insert(new_sub);
    subsId++;
};

void StompProtocol::removeSubscription(string game){
    gamesToSubId.erase(game);
};

int StompProtocol::getSubId(string &game){
    int subscriptionId = -1;
    try{
        subscriptionId = gamesToSubId.at(game);
    }catch(const std::out_of_range& oor){}
    return subscriptionId;
};

void StompProtocol::setUserName(string &name){
    userName = name;
};

string StompProtocol::getUserName(){
    return userName;
};

void StompProtocol::setLogoutReceipt(int i){
    logoutReceipt = i;
}



