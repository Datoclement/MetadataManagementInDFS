#ifndef API_HPP
#define API_HPP

#include<vector>
#include<string>

void tokenize(const std::string& line, std::vector<std::string>& argv, const std::string& charset=std::string(" \t\r\n"));

#endif
