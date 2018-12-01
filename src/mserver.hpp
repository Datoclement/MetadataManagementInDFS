#ifndef MSERVER_HPP
#define MSERVER_HPP

#include <string>
#include <vector>

#include "mconnection.hpp"
#include "msystem.hpp"

// ^^^^TODO
// make master server maintaining a list of connection with slaves
// make an interface to facilitate the one-time send message

class mServer
{
public:
    mServer(
        std::string mIP,
        int minport,
        int moutport,
        std::string MIP,
        int Minport,
        int Moutport,
        int numslaves);
    void run();
    void hislaves(std::string& placeholder);
    void sendto(const std::vector<int>& slaveids, const std::string& message, std::string& feedback);
    void slavehash(int identifier, std::vector<int>& slaveholder);

private:
    const std::string mIP;
    const int minport;
    const int moutport;
    const std::string MIP;
    const int Minport;
    const int Moutport;
    const int numslaves;
    bool is_master;
    bool slave_id;
    std::vector<std::vector<std::string> > slave_table;
    std::vector<mClientConnection*> connections;

    void initialize_master();
    void initialize_slave();
    void establish_service(int port, mSystem* msystem);
    std::string slave_str(int id);
};

#endif
