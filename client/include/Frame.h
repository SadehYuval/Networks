#pragma once
#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <list>

using std::map;
using std::string;
using std::unordered_map;
using std::vector;
using std::list;

class Frame
{
private:
    string commandLine;
    map<string, string> headers;
    string body;
    static vector<string> split(const string &str, char delimiter);

public:
    Frame(string &commandLine, map<string, string> &headers, string &body);
    string toString();
    string getCommandLine();
    map<string, string> getHeaders();
    string getBody();
    void pullFrameData(string &body, list<map<string,string>> &output);
};
