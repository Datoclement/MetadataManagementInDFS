/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
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
    // master information
    const std::string mastername = "192.168.1.164";
    const int inport = 4444;
    const int outport = 5555;
    const int numslaves = 2;

    int inportno = inport;
    int outportno = outport;

    // master should not change the port
    // slave of the same ip as master must change the ports
    if (argc == 2)
    {
        inportno = atoi(argv[1]);
    }
    else if (argc == 3)
    {
        inportno = atoi(argv[1]);
        outportno = atoi(argv[2]);
    }
    else if (argc > 2)
    {
        std::cout << "Error: too many arguments: exe [inport] [outport]" << std::endl;
    }
    std::string local_IP = get_IP("en0");
    std::cout << "my addr: " << local_IP << std::endl;
    std::cout << "my inport: " << inportno << std::endl;
    std::cout << "my outport: " << outportno << std::endl;

    mServer server(local_IP, inportno, outportno, mastername, inport, outport, numslaves);
    server.run();

    return 0;
}
