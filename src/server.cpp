#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <iostream>
#include "api.hpp"
#include "mfilesystem.hpp"
#include "mconnection.hpp"
#include "merror.hpp"
#include "goldenipcode.hpp"
#include "mserver.hpp"

#define N 512

int main(int argc, char *argv[])
{

    std::string localIP;
    std::string masterIP;
    int localport, masterport;
    int numslaves = -1;

    if (argc == 3)
    {
        localport = masterport = atoi(argv[1]);
        numslaves = atoi(argv[2]);
    }
    else if (argc == 4)
    {
        localport = atoi(argv[1]);
        masterIP = std::string(argv[2]);
        masterport = atoi(argv[3]);
    }
    else
    {
        std::string usage_msg = ""
        "Usage:\n"
        "\tfor master server: ./server PORT NUMSLAVES\n"
        "\t\tPORT     : port used to communicate with other servers\n"
        "\t\tNUMSLAVES: the number of slave server to be used\n"
        "\tfor  slave server: ./server PORT MASTERADDR MASTERPORT\n"
        "\t\tPORT      : used to communicate with the master server\n"
        "\t\tMASTERADDR: IP address of master server\n"
        "\t\tMASTERPORT: port to access master server\n";
        std::cout << usage_msg << std::endl;
        std::cout << "The addresses of this machine is: " << std::endl;
        list_IP();
        exit(0);
    }

    mServer server(localIP, localport, masterIP, masterport, numslaves);
    server.run();

    return 0;
}
