#include "mserver.hpp"
#include "mconnection.hpp"
#include "api.hpp"
#include "mfilesystem.hpp"
#include "mmetadataserver.hpp"

#include <string>
#include <vector>
#include <iostream>

using namespace std;

void flush(const string& msg)
{
    std::cout << msg;
    if (msg.size()==0 || msg.back()!='\n')
    {
        std::cout << std::endl;
    }
    std::cout.flush();
}

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
            this->slave_table.push_back(slave_profile);
            std::cout << "The " + std::to_string(i) + "-th slave is " + std::string(argv[0]) + " with its inport " + argv[1] << std::endl;
            break;
        }
    }
    for (int i=0;i<this->numslaves;i++)
    {
        std::string ip = this->slave_table[i][0];
        int port = atoi(this->slave_table[i][1].c_str());
        this->connections.push_back(new mClientConnection(ip, port));
        std::string message = "hi";
        std::string line;
        while (true)
        {
            if (!this->connections.back()->writedown(message)
             || !this->connections.back()->readin(line))
            {
                delete this->connections[i];
                this->connections[i] = new mClientConnection(ip, port);
            }
            else
            {
                break;
            }
        }
        std::cout << "Connection with " << ip << " " << port << " established." << std::endl;
    }
    std::cout << "So we can begin to work now!" << std::endl;
}

void mServer::run()
{
    if (this->is_master)
    {
        mSystem* mfs = new mFileSystem(this);
        this->establish_service(this->moutport, mfs);
    }
    else
    {
        mSystem* mms = new mMetadataServer();
        this->establish_service(this->minport, mms);
    }
}

void mServer::establish_service(int port, mSystem* sys)
{
    mServerConnection con(port);
    con.accept_connection_request();
    while (true)
    {

        std::string line;
        std::vector<std::string> argv;
        std::string message;

        if (!con.readin(line))
        {
            continue;
        }

        flush("inp: "+line);

        tokenize(line, argv);
        sys->run_command_line(argv, message);
        if (message.size() == 0)
        {
            message = "<no output>\n";
        }
        if (!con.writedown(message))
        {
            continue;
        }
        flush("out: "+message);
    }
}

void mServer::sendto(const std::vector<int>& slaveids, const string& message, string& feedback)
{
    feedback = "";
    vector<string> responses;
    for (int i=0;i<slaveids.size();i++)
    {
        int sid = slaveids[i];
        string slstr = this->slave_str(sid);
        string line;
        flush("out to " + slstr + ": " + message);
        if (!this->connections[sid]->writedown(message))
        {
            flush("Error: connection to " + slstr + " is lost when writing to it.");
            feedback += to_string(sid) + "-th slave " + slstr + " has lost the contact.\n";
        }
        else if (!this->connections[sid]->readin(line))
        {
            flush("Error: connection to " + slstr + " is lost when reading to it.");
            feedback += to_string(sid) + "-th slave " + slstr + " has lost the contact.\n";
        }
        else
        {
            responses.push_back(line);
            feedback += to_string(sid) + "-th slave " + slstr + " responds: " + line;
        }
    }
}

void mServer::hislaves(std::string& placeholder)
{
    for (int i=0;i<this->numslaves;i++)
    {
        bool connected;
        std::string message = "hi";
        std::string line;
        if (!this->connections[i]->writedown(message))
        {
            connected = false;
        }
        else if (!this->connections[i]->readin(line) || line != "hi")
        {
            connected = false;
        }
        else
        {
            connected = true;
        }
        placeholder += ( this->slave_str(i) + string(" is ") + string(connected?"yet":"not") + string(" connected\n") );
    }
}

string mServer::slave_str(int id)
{
    return this->slave_table[id][0] + " " + this->slave_table[id][1];
}

void mServer::slavehash(int identifier, std::vector<int>& slaveholder)
{
    int residu = identifier % this->numslaves;
    slaveholder.push_back(residu);
}
