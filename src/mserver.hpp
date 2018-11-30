#ifndef MSERVER_HPP
#define MSERVER_HPP

#include <string>
#include <vector>

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

    void initialize_master();
    void initialize_slave();
    void run_master();
    void run_slave();
};

#endif
