#pragma once

#ifndef MFILESYSTEM_HPP
#define MFILESYSTEM_HPP

#include <vector>
#include <string>

#include "msystemtree.hpp"
#include "mserver.hpp"
#include "msystem.hpp"

class mSystemTree;

class mFileSystem : public mSystem
{
public:
    mServer* owner;
    mFileSystem(mServer* owner);
    void run_command_line(const std::vector<std::string>& argv, std::string& placeholder);
    ~mFileSystem();

private:
    mSystemTree* systemtree;

    void pwd(const std::vector<std::string>& argv, std::string& placeholder);
    void mkdir(const std::vector<std::string>& argv, std::string& placeholder);
    void ls(const std::vector<std::string>& argv, std::string& placeholder);
    void readdir(const std::vector<std::string>& argv, std::string& placeholder);
    void cd(const std::vector<std::string>& argv, std::string& placeholder);
    void mv(const std::vector<std::string>& argv, std::string& placeholder);
    void stat(const std::vector<std::string>& argv, std::string& placeholder);
    void rm(const std::vector<std::string>& argv, std::string& placeholder);
    void touch(const std::vector<std::string>& argv, std::string& placeholder);
    void checkservers(const std::vector<std::string>& argv, std::string& placeholder);
    void port(const std::vector<std::string>& argv, std::string& placeholder);
};

#endif
