
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
#include <boost/algorithm/string/trim.hpp>
#include <iostream>
#include <fstream>
#include <mutex>



using std::vector;
using std::string;
using std::stringstream;
using std::unordered_map;
using std::pair;
using std::map;
using namespace std;
std::mutex mtx;


StompProtocol::StompProtocol(): gamesToSubId(), userName(""), subsId(0), reportsMap(),should_terminate(false),logoutReceipt(-1), connected(false){};



void StompProtocol::summaryProcess(Frame &frame){
    mtx.lock();
    string game = frame.getHeaders().at("game");
    string userName = frame.getHeaders().at("user");
    string txtFile = frame.getHeaders().at("file");
    try{
        gamesToSubId.at(game);
        try{
            pair<string,string> gameToUser(game,userName); 
            list<Frame> updates = reportsMap.at(gameToUser);
            map<string,string> general_game_updates;
            map<string,string> team_a_updates;
            map<string,string> team_b_updates;
            list<map<string,string>> game_events;
            for(auto &frame : updates){
                string body = frame.getBody();
                list<map<string,string>> temp;
                frame.pullFrameData(body, temp);
                map<string,string> g_g_u = temp.front();
                temp.pop_front();
                for(const auto &update : g_g_u){
                    string key = update.first;
                    string value = update.second;
                    general_game_updates.insert_or_assign(key, value);
                }
                map<string,string> t_a_u = temp.front();
                temp.pop_front();
                for(const auto &update : t_a_u){
                    string key = update.first;
                    string value = update.second;
                    team_a_updates.insert_or_assign(key, value);
                }
                map<string,string> t_b_u = temp.front();
                temp.pop_front();
                for(const auto &update : t_b_u){
                    string key = update.first;
                    string value = update.second;
                    team_b_updates.insert_or_assign(key, value);
                }
                map<string,string> g_e = temp.front();
                temp.pop_front();
                game_events.push_back(g_e);
            }           
            //creat the file 
            std::ofstream file(txtFile);
            size_t position = game.find('_');
            file << game.substr(0,position) + " vs " + game.substr(position +1) + '\n';
            file << "Game stats:\n";
            file << "General stats:\n";
            for(const auto &update : general_game_updates){
                file << update.first + ": " + update.second  + '\n';
            }
            file << game.substr(0,position) + " stats:\n";
            for(const auto &update : team_a_updates){
                file << update.first + ": " + update.second  + '\n';
            }
            file << game.substr(position + 1) + " stats:\n";
            for(const auto &update : team_b_updates){
                file << update.first + ": " + update.second  + '\n';
            }
            file <<  "Game event reports:\n";
            for(const auto &map : game_events){
                file << map.at("time") + " - " + map.at("event name") + "\n\n";
                file << map.at("description") + "\n\n\n";
            }
            file.close();

        }catch(const std::out_of_range& noReports){
            //user not reported on this game
            //return an empty file
            std::ofstream file(txtFile);
            std::cout << "user not reported on this game" << std::endl;
        }

    }catch(const std::out_of_range& unSubscribed){
        //user not subscribed to this game
        std::cout << "user not subscribed to this game" << std::endl;
    }
    mtx.unlock();
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
        std::getline(ss, userName, '\n');
        std::size_t position = userName.find(':');
        userName = userName.substr(position +1);
        boost::trim(userName);
        string game = frame.getHeaders().at("destination");
        game = game.substr(1);
        pair<string,string> temp (game, userName);
        list<Frame>* updates;
        mtx.lock();
        try{
            updates = &reportsMap.at(temp);
            updates->push_back(frame);

        }catch(const std::out_of_range& oor){
            list<Frame> newUpdates;
            newUpdates.push_back(frame);
            pair<pair<string,string>,list<Frame>> insert (temp, newUpdates);
            reportsMap.insert(insert);   
        }
        mtx.unlock();
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



