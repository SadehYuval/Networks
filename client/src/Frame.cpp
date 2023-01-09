
#include "Frame.h"
#include <sstream>
#include <string>
#include <vector>
#include <map>

using std::string;
using std::map;
using std::vector;
using std::pair;



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
    output = output + '\0';
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