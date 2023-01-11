
#include "Frame.h"
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <boost/algorithm/string/trim.hpp>

using std::string;
using std::map;
using std::vector;
using std::pair;
using std::list;
using std::size_t;



Frame::Frame(string &commandLine, map<string, string> &headers, string &body): commandLine(commandLine), headers(headers), body(body){
    
};

string Frame::toString(){
    string output = "";
    output = output + commandLine + '\n';
    for(auto header: headers){
        output = output + header.first + ":" + header.second + '\n';
    }
    output = output + '\n';
    output = output + body;
    return output;
};



vector<string> Frame::split(const string& str, char delimiter) {
  vector<string> lines;
  std::stringstream ss(str);
  string line;
  while (std::getline(ss, line, delimiter)) {
    lines.push_back(line);
  }
  return lines;
}

string Frame::getCommandLine(){
  return commandLine;
};

string Frame::getBody(){
  return body;
}

map<string, string> Frame::getHeaders(){
  return headers;
}

void Frame::pullFrameData(string &body, list<map<string,string>> &output){
    map<string,string> general_game_updates;
    map<string,string> team_a_updates;
    map<string,string> team_b_updates;
    map<string,string> game_events;
    vector<string> lines = split(body, '\n');
    int bodyLength = lines.size();
    int index = 0;
    while(index < bodyLength && lines[index].compare("general game updates:")!=0 ){
      size_t position = lines[index].find(':');
      string stat_name = lines[index].substr(0, position);
      boost::trim(stat_name);
      string stat_value = lines[index].substr(position +1);
      boost::trim(stat_value);
      if(stat_name.compare("time") == 0){
        pair<string,string> time (stat_name, stat_value);
        game_events.insert(time);
      }
      if(stat_name.compare("event name") == 0){
        pair<string,string> event_name (stat_name, stat_value);
        game_events.insert(event_name);
      }
      index++;
    }
    index++;
    while(index < bodyLength && lines[index].compare("team a updates:") != 0){
      size_t position = lines[index].find(':');
      string stat_name = lines[index].substr(0, position);
      boost::trim(stat_name);
      string stat_value = lines[index].substr(position +1);
      boost::trim(stat_value);
      pair<string, string> toAdd (stat_name, stat_value);
      general_game_updates.insert(toAdd);
      index++;
    }
    index++;
    while(index < bodyLength && lines[index].compare("team b updates:") != 0){
      size_t position = lines[index].find(':');
      string stat_name = lines[index].substr(0, position);
      boost::trim(stat_name);
      string stat_value = lines[index].substr(position +1);
      boost::trim(stat_value);
      pair<string, string> toAdd (stat_name, stat_value);
      team_a_updates.insert(toAdd);
      index++;
    }
    index++;
    while(index < bodyLength && lines[index].compare("description:") != 0){
      size_t position = lines[index].find(':');
      string stat_name = lines[index].substr(0, position);
      boost::trim(stat_name);
      string stat_value = lines[index].substr(position +1);
      boost::trim(stat_value);
      pair<string, string> toAdd (stat_name, stat_value);
      team_b_updates.insert(toAdd);
      index++;
    }
    index++;
    string desc = lines[index];
    boost::trim(desc);
    pair<string,string> description ("description",desc);
    game_events.insert(description);
    output.push_back(general_game_updates);
    output.push_back(team_a_updates);
    output.push_back(team_b_updates);
    output.push_back(game_events);
};