#include "mmetadataserver.hpp"
#include "merror.hpp"
#include "msystem.hpp"

#include <vector>
#include <string>
#include <iostream>

using namespace std;

mMetadataServer::mMetadataServer():metadata_map()
{}

mMetadataServer::~mMetadataServer()
{
    for (auto it = metadata_map.begin(); it != metadata_map.end(); it++)
    {
        delete it->second;
        metadata_map.erase(it);
    }
}

void mMetadataServer::hi(const vector<string>& argv, string& placeholder)
{
    placeholder = "hi";
}

void mMetadataServer::run_command_line(const vector<string>& argv, string& placeholder)
{
    if (argv.size() == 0)
    {
        error("argv size should never be zero. debug it.\n");
    }
    const string& command = argv[0];
    if (command == "hi")
    {
        this->hi(argv, placeholder);
    }
    else if (command == "request")
    {
       this->request(argv, placeholder);
    }
    else if (command == "delete")
    {
        this->remove(argv, placeholder);
    }
    else if (command == "create")
    {
      this->create(argv, placeholder);
    }
    else if (command == "update")
    {
        this->update(argv, placeholder);
    }
    else
    {
        placeholder = "Error: command " + command + " is not supported.\n";
    }
    std::cout << "------map------" << std::endl;
    for (auto it=metadata_map.begin();it!=metadata_map.end();it++)
    {
        std::cout << it->second->summary() << std::endl;
    }
}

void mMetadataServer::request(const std::vector<std::string>& argv, std::string& placeholder)
{
    if (argv.size() != 2)
    {
        placeholder = "Error: wrong argument number for request.\n";
        return;
    }
    int id = atoi(argv[1].c_str());
    auto it = metadata_map.find(id);
    if (it == metadata_map.end())
    {
        placeholder = "Error: request failed.";
        return;
    }
    placeholder = it->second->summary();
}

void mMetadataServer::remove(const std::vector<std::string>& argv, std::string& placeholder)
{
    if (argv.size() != 2)

    {
        placeholder = "Error: wrong argument number for delete.";
        return;
    }
    int

    id = atoi(argv[1].c_str());
    auto it = metadata_map.find(id);
    if (it == metadata_map.end())
    {
        placeholder = "Error: delete failed.";
        return;
    }
    delete it->second;
    metadata_map.erase(it);
    placeholder = "Success";
}

void mMetadataServer::create(const std::vector<std::string>& argv, std::string& placeholder)
{
    if (argv.size() != 8)
    {
        placeholder = "Error: wrong argument number for delete.";
        return;
    }
    int id = atoi(argv[1].c_str());
    int parent = atoi(argv[2].c_str());
    const std::string& creation_time = argv[3];
    const std::string& lastmodify_time = argv[4];
    size_t size = atoi(argv[5].c_str());
    const std::string& name = argv[6];
    const int type = atoi(argv[7].c_str());
    mMetadata* new_metadata = new mMetadata(id, parent, creation_time, size,
                                            lastmodify_time, name, type);
    auto it = metadata_map.find(id);
    if (it != metadata_map.end())
    {
        placeholder = "Error: file id already exists.";
        return;
    }
    metadata_map.insert(std::pair<int, mMetadata*>(id, new_metadata));
    placeholder = "Success";
}

void mMetadataServer::update(const std::vector<std::string>& argv, std::string& placeholder)
{
    if ((argv.size()) % 2 != 0)
    {
        placeholder = "Error: incomplete key value pairs.";
        return;
    }
    int id = atoi(argv[1].c_str());
    auto it = metadata_map.find(id);
    if (it == metadata_map.end())
    {
        placeholder = "Error: file id doesn't exist.";
        return;
    }
    mMetadata* metadata = metadata_map[id];
    for (int i = 2; i < argv.size(); i+=2)
    {

        if (argv[i] == "parent")
        {
            metadata->updata_parent(atoi(argv[i+i].c_str()));
        }
        else if (argv[i] == "lastmodify_time")
        {
            metadata->update_lastmodify_time(argv[i+1]);
        }
        else if (argv[i] == "name")
        {
            metadata->update_name(argv[i+1]);
        }
        else
        {
            placeholder = "Error: unknown key value.";
            return;
        }
        placeholder = "Success";
    }
}
