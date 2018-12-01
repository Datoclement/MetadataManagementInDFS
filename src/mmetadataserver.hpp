#ifndef MMETADATASERVER_HPP
#define MMETADATASERVER_HPP

#include <string>
#include <vector>

#include "msystem.hpp"

class mMetadataServer : public mSystem
{
public:
    mMetadataServer();
    void run_command_line(const std::vector<std::string>& argv, std::string& placeholder);
    void hi(const std::vector<std::string>& argv, std::string& placeholder);

};

#endif
