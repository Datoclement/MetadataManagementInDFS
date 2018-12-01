#include <map>
#include <vector>
#include <string>
#include <iostream>

#include "mfilesystem.hpp"
#include "merror.hpp"
#include "mserver.hpp"
#include "msystem.hpp"

using namespace std;

bool is_option_kw(const string& arg)
{
    if (arg.size() == 0)
    {
        error("Error: arg should never be empty string. debug it.");
    }
    return arg[0] == '-';
}

mFileSystem::mFileSystem(mServer* owner)
{
    this->systemtree = new mSystemTree(this);
    this->owner = owner;
}

mFileSystem::~mFileSystem()
{
    delete this->systemtree;
}

void mFileSystem::pwd(const vector<string>& argv, string& placeholder)
{
    this->systemtree->get_working_directory(placeholder);
}

void mFileSystem::mkdir(const vector<string>& argv, string& placeholder)
{
    if (argv.size() < 2)
    {
        placeholder = "Error: mkdir accepts 1 argument.\n";
    }
    else
    {
        const string& name = argv[1];
        this->systemtree->make_directory(name, placeholder);
    }
}

void mFileSystem::ls(const vector<string>&argv, string& placeholder)
{
    string src = ".";
    bool recursive = false;
    int len = argv.size();
    for (int i=1;i<len;i++)
    {
        if (is_option_kw(argv[i]))
        {
            if (argv[i] == "-r")
            {
                recursive = true;
            }
            else
            {
                placeholder = "Error: option/argument " + argv[i] + " is not supported.\n";
            }
        }
        else
        {
            src = argv[i];
        }
    }

    this->systemtree->list(src, recursive, placeholder);
}

void mFileSystem::readdir(const vector<string>& argv, string& placeholder)
{
    string src = ".";
    if (argv.size() > 1)
    {
        src = argv[1];
    }
    this->systemtree->list(argv[1], false, placeholder);
}

void mFileSystem::cd(const vector<string>& argv, string& placeholder)
{
    if (argv.size() == 1)
    {
        placeholder = "Error: cd accepts 1 argument.\n";
    }
    else
    {
        this->systemtree->change_directory(argv[1], placeholder);
    }
}

void mFileSystem::mv(const vector<string>& argv, string& placeholder)
{
    if (argv.size() < 3)
    {
        placeholder = "Error: mv accepts 2 argument.\n";
    }
    else
    {
        this->systemtree->move(argv[1], argv[2], placeholder);
    }
}

void mFileSystem::stat(const vector<string>& argv, string& placeholder)
{
    if (argv.size() == 1)
    {
        placeholder = "Error: stat accepts 1 argument.\n";
    }
    else
    {
        this->systemtree->state(argv[1], placeholder);
    }
}

void mFileSystem::touch(const vector<string>& argv, string& placeholder)
{
    if (argv.size() == 1)
    {
        placeholder = "Error: touch accepts 1 argument.\n";
    }
    else
    {
        this->systemtree->touch(argv[1], placeholder);
    }
}

void mFileSystem::rm(const vector<string>& argv, string& placeholder)
{
    string src = "";
    bool recursive = false;
    int len = argv.size();
    for (int i=1;i<len;i++)
    {
        if (is_option_kw(argv[i]))
        {
            if (argv[i] == "-r")
            {
                recursive = true;
            }
            else
            {
                placeholder = "Error: option/argument " + argv[i] + " is not supported.\n";
            }
        }
        else
        {
            src = argv[i];
        }
    }
    if (src == "")
    {
        placeholder = "Error: rm accepts 1 positional arguments.\n";
    }
    else
    {
        this->systemtree->remov(src, recursive, placeholder);
    }
}

void mFileSystem::checkservers(const vector<string>& argv, string& placeholder)
{
    this->owner->hislaves(placeholder);
}

void mFileSystem::port(const vector<string>& argv, string& placeholder)
{
    this->owner->port_replacement(argv, placeholder);
}

void mFileSystem::run_command_line(const vector<string>& argv, string& placeholder)
{
    if (argv.size() == 0)
    {
        error("Error: argv size should never be zero. debug it.\n");
    }
    const string& command = argv[0];
    if (command == "pwd")
    {
        this->pwd(argv, placeholder);
    }
    else if (command == "mkdir")
    {
        this->mkdir(argv, placeholder);
    }
    else if (command == "ls")
    {
        this->ls(argv, placeholder);
    }
    else if (command == "readdir")
    {
        this->readdir(argv, placeholder);
    }
    else if (command == "cd")
    {
        this->cd(argv, placeholder);
    }
    else if (command == "mv")
    {
        this->mv(argv, placeholder);
    }
    else if (command == "stat")
    {
        this->stat(argv, placeholder);
    }
    else if (command == "touch")
    {
        this->touch(argv, placeholder);
    }
    else if (command == "rm")
    {
        this->rm(argv, placeholder);
    }
    else if (command == "checkservers")
    {
        this->checkservers(argv, placeholder);
    }
    else if (command == "port")
    {
        this->port(argv, placeholder);
    }
    else
    {
        placeholder = "Error: command " + command + " is not supported.\n";
    }
}
