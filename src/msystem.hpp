#ifndef MSYSTEM_HPP
#define MSYSTEM_HPP

#include <string>
#include <vector>

class mSystem
{
public:
    virtual void run_command_line(const std::vector<std::string>& argv, std::string& placeholder) = 0;
};

#endif
