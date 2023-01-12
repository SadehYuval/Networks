#pragma once
#include <string>
#include <vector>
#include "event.h"
#include <list>
#include "Frame.h"

using std::string;
using std::pair;
using std::vector;
using std::list;
using std::map;


class StompProtocol
{
private:
    map<string, int> gamesToSubId;
    string userName;
    int subsId;
    map<pair<string,string>, list<Frame>> reportsMap;
    list<map<string,string>> pullFrameData(Frame &update);
    



public:
    StompProtocol();
    bool should_terminate;
    void receiveProcess(Frame &frame);
    void summaryProcess(Frame &frame);

    
    void addSubscription(string game);
    void removeSubscription(string game);
    int getSubId(string &game);
    void setUserName(string &name);
    string getUserName();
    void setLogoutReceipt(int i);
    int logoutReceipt;
    bool connected;

};
