#ifndef MMETADATASERVER_HPP
#define MMETADATASERVER_HPP

#include <string>
#include <vector>
#include <map>

#include "mserver.hpp"
#include "mmetadata.hpp"
#include "msystem.hpp"

class mMDTLogicSystem : public mSystem
{
public:
    mMDTLogicSystem(mServer* owner);
    ~mMDTLogicSystem();
    void run_command_line(const std::vector<std::string>& argv, std::string& placeholder);
private:
    mServer* owner;
    std::map<int, mMetadata*> metadata_map;
    void hi(const std::vector<std::string>& argv, std::string& placeholder);
    void request(const std::vector<std::string>& argv, std::string& placeholder);
    void remove(const std::vector<std::string>& argv, std::string& placeholder);
    void create(const std::vector<std::string>& argv, std::string& placeholder);
    void update(const std::vector<std::string>& argv, std::string& placeholder);
    void pushto(const std::vector<std::string>& argv, std::string& placeholder);
};

#endif
