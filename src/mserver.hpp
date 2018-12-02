#ifndef MSERVER_HPP
#define MSERVER_HPP

#include <string>
#include <vector>

#include "mconnection.hpp"
#include "msystem.hpp"

class mServer
{
public:
    mServer(
        std::string localIP,
        int localport,
        std::string masterIP,
        int masterport,
        int numslaves);
    void run();
    void hislaves(std::string& placeholder);
    void port_replacement(const std::vector<std::string>& argv, std::string& placeholder);
    void sendto(const std::vector<int>& slaveids, const std::string& message, std::string& feedback);
    void slavehash(int identifier, std::vector<int>& slaveholder);

private:
    mServerConnection* scon;
    std::string localIP;
    const int localport;
    const std::string masterIP;
    const int masterport;
    const int numslaves;
    bool is_master;
    bool slave_id;
    std::vector<std::vector<std::string> > slave_table;
    std::vector<mClientConnection*> connections;
    std::vector<bool> connecteds;
    std::vector<bool> replacements;

    void initialize_master();
    void initialize_slave();
    void establish_service(mSystem* msystem);
    std::string slave_str(int id);
};

#endif
