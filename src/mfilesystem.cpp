#include <map>
#include <vector>
#include <string>
#include <iostream>

#include "mfilesystem.hpp"
#include "merror.hpp"

using namespace std;

bool is_option_kw(string& arg)
{
    if (arg.size() == 0)
    {
        error("Error: arg should never be empty string. debug it.");
    }
    return arg[0] == '-';
}

mFileSystem::mFileSystem()
{
    this->systemtree = new mSystemTree();
}

mFileSystem::~mFileSystem()
{
    delete this->systemtree;
}

void mFileSystem::pwd(vector<string>& argv, string& placeholder)
{
    this->systemtree->get_working_directory(placeholder);
}

void mFileSystem::mkdir(vector<string>& argv, string& placeholder)
{
    if (argv.size() < 2)
    {
        placeholder = "Error: mkdir accepts 1 argument.\n";
    }
    else
    {
        string& name = argv[1];
        this->systemtree->make_directory(name, placeholder);
    }
}

void mFileSystem::ls(vector<string>&argv, string& placeholder)
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

void mFileSystem::readdir(vector<string>& argv, string& placeholder)
{
    string src = ".";
    if (argv.size() > 1)
    {
        src = argv[1];
    }
    this->systemtree->list(argv[1], false, placeholder);
}

void mFileSystem::cd(vector<string>& argv, string& placeholder)
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

void mFileSystem::mv(vector<string>& argv, string& placeholder)
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

void mFileSystem::stat(vector<string>& argv, string& placeholder)
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

void mFileSystem::touch(vector<string>& argv, string& placeholder)
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

void mFileSystem::rm(vector<string>& argv, string& placeholder)
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

void mFileSystem::rg(vector<string>& argv, string& placeholder)
{
    // string path = ".";
    // if (argv.size() >= 1)
    // {
    //     path = argv[1];
    // }
    // this->systemtree->random_generate_file();
}

void mFileSystem::checkservers(vector<string>& argv, string& placeholder)
{
    
}

void mFileSystem::run_command_line(vector<string>& argv, string& placeholder)
{
    if (argv.size() == 0)
    {
        error("argv size should never be zero. debug it.\n");
    }
    string& command = argv[0];
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
    else if (command == "rg")
    {
        this->rg(argv, placeholder);
    }
    else if (command == "checkservers")
    {
        this->checkservers(argv, placeholder);
    }
    else
    {
        placeholder = "Error: command " + command + " is not supported.\n";
    }
}
