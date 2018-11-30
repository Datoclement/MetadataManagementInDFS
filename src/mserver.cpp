#include "mserver.hpp"
#include "mconnection.hpp"
#include "api.hpp"
#include "mfilesystem.hpp"
#include "mmetadataserver.hpp"

#include <string>
#include <vector>
#include <iostream>

using namespace std;

mServer::mServer(string mIP, int minport, int moutport, string MIP, int Minport, int Moutport, int numslaves):
    mIP(mIP),
    minport(minport),
    moutport(moutport),
    MIP(MIP),
    Minport(Minport),
    Moutport(Moutport),
    numslaves(numslaves)
{
    this->is_master = (this->mIP == this->MIP) && (this->minport == this->Minport);

    if (is_master)
    {
        this->initialize_master();
    }
    else
    {
        this->initialize_slave();
    }
}

void mServer::initialize_slave()
{
    std::cout << "I am a slave." << std::endl;
    std::cout << "I am going to work for the master " + this->MIP + " with his port " + std::to_string(this->Minport) << std::endl;
    mClientConnection con(this->MIP, this->Minport);
    std::string message(this->mIP + " " + std::to_string(this->minport));
    while (true)
    {
        if (!con.writedown(message))
        {
            con = mClientConnection(this->MIP, this->Minport);
            continue;
        }
        std::string line;
        if (!con.readin(line))
        {
            con = mClientConnection(this->MIP, this->Minport);
            continue;
        }
        this->slave_id = atoi(line.c_str());
        std::cout << "I am now registered as the " + std::to_string(this->slave_id) + "-th slave of the master." << std::endl;
        break;
    }
    std::cout << "Ready to work!" << std::endl;
}

void mServer::initialize_master()
{
    std::cout << "I am the mighty master." << std::endl;
    std::cout << "I am waiting for " << this->numslaves << " slaves to store the metadata." << std::endl;
    mServerConnection con(this->minport);
    for (int i=0;i<this->numslaves;i++)
    {
        while (true)
        {
            con.accept_connection_request();
            std::string line;
            std::vector<std::string> argv;
            if (!con.readin(line))
            {
                continue;
            }
            tokenize(line, argv);
            std::string message = std::to_string(i);
            if (!con.writedown(message))
            {
                continue;
            }
            std::vector<std::string> slave_profile;
            slave_profile.push_back(std::string(argv[0]));
            slave_profile.push_back(std::string(argv[1]));
            slave_table.push_back(slave_profile);
            std::cout << "The " + std::to_string(i) + "-th slave is " + std::string(argv[0]) + " with its inport " + argv[1] << std::endl;
            break;
        }
    }
    std::cout << "So we can begin to work now!" << std::endl;
}

void mServer::run()
{
    if (this->is_master)
    {
        this->run_master();
    }
    else
    {
        this->run_slave();
    }
}

void mServer::run_master()
{
    mServerConnection con(this->moutport);
    con.accept_connection_request();
    mFileSystem mfs(this);

    while (true)
    {

        std::string line;
        std::vector<std::string> argv;
        std::string message;

        if (!con.readin(line))
        {
            continue;
        }

        tokenize(line, argv);
        mfs.run_command_line(argv, message);
        if (message.size() == 0)
        {
            message = "<no output>\n";
        }
        if (!con.writedown(message))
        {
            continue;
        }
    }
}

void mServer::run_slave()
{
    mServerConnection con(this->minport);
    con.accept_connection_request();
    mMetadataServer mms;

    while (true)
    {

        std::string line;
        std::vector<std::string> argv;
        std::string message;

        if (!con.readin(line))
        {
            continue;
        }

        tokenize(line, argv);
        mms.run_command_line(argv, message);
        if (message.size() == 0)
        {
            message = "<no output>\n";
        }
        if (!con.writedown(message))
        {
            continue;
        }
    }
}

void mServer::hislaves(std::string& placeholder)
{
    for (int i=0;i<this->numslaves;i++)
    {
        bool connected = true;
        std::string addr = this->slave_table[i][0];
        int port = atoi(this->slave_table[i][1].c_str());
        mClientConnection con(addr, port);
        std::string message = "hi";
        if (!con.writedown(message))
        {
            connected = false;
        }
        else
        {
            std::string line;
            if (!con.readin(line))
            {
                connected = false;
            }
        }
        placeholder += (
            addr + string(" ") + to_string(port) + string(" is ") + string(connected?"yet":"not") + string(" connected\n") );
    }
}
