#include <iostream>
#include <vector>
#include <string>

#include "api.hpp"

using namespace std;

void tokenize(const string& line, vector<string>& argv, const string& charset)
{
    size_t len = line.size();
    size_t pos = 0;
    while (true)
    {
        if (pos == len)
        {
            break;
        }
        else
        {
            size_t next = line.find_first_of(charset.c_str(), pos);
            if (next == -1)
            {
                argv.push_back(line.substr(pos, len-pos));
                pos = len;
            }
            else if (next == pos)
            {
                pos += 1;
            }
            else
            {
                argv.push_back(line.substr(pos, next-pos));
                pos = next;
            }
        }
    }
}
