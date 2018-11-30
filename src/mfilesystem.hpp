#ifndef MFILESYSTEM_HPP
#define MFILESYSTEM_HPP

#include <vector>
#include <string>

#include "msystemtree.hpp"

class mFileSystem
{
public:
    mFileSystem();
    ~mFileSystem();
    void run_command_line(std::vector<std::string>& argv, std::string& placeholder);

private:
    mSystemTree* systemtree;
    // std::vector<
    void pwd(std::vector<std::string>& argv, std::string& placeholder);
    void mkdir(std::vector<std::string>& argv, std::string& placeholder);
    void ls(std::vector<std::string>& argv, std::string& placeholder);
    void readdir(std::vector<std::string>& argv, std::string& placeholder);
    void cd(std::vector<std::string>& argv, std::string& placeholder);
    void mv(std::vector<std::string>& argv, std::string& placeholder);
    void touch(std::vector<std::string>& argv, std::string& placeholder);
    void stat(std::vector<std::string>& argv, std::string& placeholder);
    void rm(std::vector<std::string>& argv, std::string& placeholder);
    void rg(std::vector<std::string>& argv, std::string& placeholder);
    void checkservers(std::vector<std::string>& argv, std::string& placeholder);
};

#endif
