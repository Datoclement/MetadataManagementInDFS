#include "mserver.hpp"
#include "mconnection.hpp"
#include "api.hpp"
#include "mmdslogicsystem.hpp"
#include "mmdtlogicsystem.hpp"
#include "merror.hpp"

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

mServer::mServer(string localIP, int localport, string masterIP, int masterport, int numslaves):
    localIP(localIP),
    localport(localport),
    masterIP(masterIP),
    masterport(masterport),
    numslaves(numslaves)
{
    this->is_master = (this->localIP == this->masterIP) && (this->localport == this->masterport);

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
    std::cout << "I am going to work for the master " + this->masterIP + " with his port " + std::to_string(this->masterport) << std::endl;
    mClientConnection con(this->masterIP, this->masterport);
    std::string message("port " + std::to_string(this->localport));
    while (true)
    {
        std::string line;
        if (con.writedown(message) && con.readin(line))
        {
            std::vector<std::string> tokens;
            tokenize(line, tokens);
            if (tokens[0] == "Error:")
            {
                error("No need for replacement.");
            }
            this->slave_id = atoi(tokens[0].c_str());
            this->localIP = tokens[1];
            std::cout << "I am now registered as the " + std::to_string(this->slave_id) + "-th slave of the master and get my IP address " + this->localIP << std::endl;
            break;
        }
        else
        {
            con = mClientConnection(this->masterIP, this->masterport);
        }
    }
    this->scon = new mServerConnection(this->localport);
    std::cout << "Ready to work!" << std::endl;
}

void mServer::initialize_master()
{
    std::cout << "I am the mighty master." << std::endl;
    std::cout << "I am waiting for " << this->numslaves << " slaves to store the metadata." << std::endl;
    this->scon = new mServerConnection(this->localport);
    for (int i=0;i<this->numslaves;i++)
    {
        while (true)
        {
            this->scon->accept_connection_request();
            std::string message = std::to_string(i) + " " + this->scon->get_client_ip_addr();
            std::string line;
            std::vector<std::string> argv;
            if (!(this->scon->readin(line) && this->scon->writedown(message)))
            {
                continue;
            }
            else
            {
                std::vector<std::string> tokens;
                tokenize(line, tokens);
                if (tokens[0] != "port")
                {
                    continue;
                }
                else
                {
                    std::vector<std::string> slave_profile;
                    slave_profile.push_back(this->scon->get_client_ip_addr());
                    slave_profile.push_back(tokens[1]);
                    this->slave_table.push_back(slave_profile);
                    std::cout << "The " + std::to_string(i) + "-th slave is " + slave_profile[0] + " with its port " + slave_profile[1] << std::endl;
                    break;
                }
            }
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
            if (this->connections.back()->writedown(message)
             && this->connections.back()->readin(line))
            {
                break;
            }
            else
            {
                delete this->connections[i];
                this->connections[i] = new mClientConnection(ip, port);
            }
        }
        this->connecteds.push_back(true);
        this->replacements.push_back(false);
        std::cout << "Connection with " << ip << " " << port << " established." << std::endl;
    }
    std::cout << "We can begin to serve now!" << std::endl;
}

void mServer::run()
{
    if (this->is_master)
    {
        mSystem* mds = new mMDSLogicSystem(this);
        this->establish_service(mds);
    }
    else
    {
        mSystem* mms = new mMDTLogicSystem(this);
        this->establish_service(mms);
    }
}

void mServer::establish_service(mSystem* sys)
{
    this->scon->accept_connection_request();
    while (true)
    {

        std::string line;
        std::vector<std::string> argv;
        std::string message;

        if (!this->scon->readin(line))
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
        if (!this->scon->writedown(message))
        {
            continue;
        }
        flush("out: "+message);
    }
}

void mServer::connectto(int sid)
{
    std::string ip = this->slave_table[sid][0];
    int port = atoi(this->slave_table[sid][1].c_str());
    this->connections[sid] = new mClientConnection(ip, port);
}

bool mServer::synchronize_data(int src, int dst)
{
    std::string message = "pushto " + this->slave_table[dst][0] + " " + this->slave_table[dst][1];
    std::vector<int> slaveidarr; slaveidarr.push_back(src);
    std::string line;
    delete this->connections[dst];
    this->sendto(slaveidarr, message, line);
    this->connectto(dst);
    std::vector<std::string> lines;
    tokenize(line, lines, "\n");
    std::vector<std::string> tokens;
    tokenize(lines[1], tokens);
    return tokens.size() == 3 && tokens[2] == "Success";
}

bool mServer::recover(int id)
{
    int cnt = 0;
    for (int i=1;i<this->numslaves;i++)
    {
        int sid = (id+1) % this->numslaves;
        if (this->connecteds[sid] && this->synchronize_data(sid, id))
        {
            cnt++;
            if (cnt >= 2)
            {
                break;
            }
        }
    }
    return (cnt >= 2) || ((cnt == 1) && this->numslaves == 2);
}

void mServer::sendto(const std::string& IP, int port, const std::string& message, std::string& placeholder)
{
    mClientConnection con(IP, port);
    std::string line;
    if (!con.writedown(message)
     || !con.readin(placeholder))
    {
        placeholder = "Failure";
    }
}

void mServer::sendto(const std::vector<int>& slaveids, const string& message, string& feedback)
{
    vector<string> responses;
    for (int i=0;i<slaveids.size();i++)
    {
        int sid = slaveids[i];
        string slstr = this->slave_str(sid);
        string line;
        flush("out to " + slstr + ": " + message);
        if (!this->connecteds[sid] && !this->replacements[sid])
        {
            responses.push_back("");
            feedback += to_string(sid) + "-th slave " + slstr + " is still dead\n";
        }
        else if (this->replacements[sid])
        {
            responses.push_back("");
            delete this->connections[sid];
            this->connectto(sid);
            if (!this->recover(sid))
            {
                feedback += to_string(sid) + "-th slave " + slstr + " recovery failed\n";
                flush("Successful connection but failed recovery to " + slstr + ".");
            }
            else
            {
                this->connecteds[sid] = true;
                this->replacements[sid] = false;

                feedback += to_string(sid) + "-th slave " + slstr + " finds a replacement\n";
                flush("Successful connection and recovery to " + slstr + ".");
            }
        }
        else if (this->connections[sid]->writedown(message)
         && this->connections[sid]->readin(line))
        {
            responses.push_back(line);
            feedback += to_string(sid) + "-th slave " + slstr + " responds: " + line + "\n";
            flush("in from " + slstr + ": " + line);
        }
        else
        {
            responses.push_back("");
            this->connecteds[sid] = false;
            flush("Error: connection to " + slstr + " is lost.");
            feedback += to_string(sid) + "-th slave " + slstr + " has lost the contact.\n";
        }
    }

    string formal_output;
    for (int i=0;i<slaveids.size();i++)
    {
        if (responses[i] == "")
        {
            continue;
        }
        else
        {
            formal_output = responses[i];
            vector<string> tokens;
            tokenize(responses[i], tokens);
            if (tokens[0] != "Error:")
            {
                break;
            }
        }
    }
    feedback += "formal output: " + formal_output + "\n";
}

void mServer::hislaves(std::string& placeholder)
{
    for (int i=0;i<this->numslaves;i++)
    {
        std::string message = "hi";
        std::string line;
        bool connected = this->connections[i]->writedown(message)
                      && this->connections[i]->readin(line)
                      && line == "hi";
        placeholder += ( this->slave_str(i) + string(" is ") + string(connected?"yet":"not") + string(" connected\n") );
    }
}

string mServer::slave_str(int id)
{
    return "[" + to_string(id) + ", " + this->slave_table[id][0] + ", " + this->slave_table[id][1] + "]";
}

void mServer::slavehash(int identifier, std::vector<int>& slaveholder)
{
    if (this->numslaves == 2)
    {
        slaveholder.push_back(0);
        slaveholder.push_back(1);
    }
    else
    {
        int residu = identifier % this->numslaves;
        for (int i=0;i<this->numslaves;i++)
        {
            if (residu != i)
            slaveholder.push_back(i);
        }
    }
}

void mServer::port_replacement(const std::vector<std::string>& argv, std::string& placeholder)
{
    for (int i=0;i<this->numslaves;i++)
    {
        if (!this->connecteds[i])
        {
            this->slave_table[i][0] = this->scon->get_client_ip_addr();
            this->slave_table[i][1] = argv[1];
            this->replacements[i] = true;
            placeholder += to_string(i) + " " + this->slave_table[i][0];
            return;
        }
    }
    placeholder += "Error: num of active slaves is at maximum.\n";
}
