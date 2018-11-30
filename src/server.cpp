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

#define N 512

int main(int argc, char *argv[])
{
    // master information
    const std::string mastername = "192.168.1.162";
    const int port = 4444;
    const int numslaves = 1;

    int portno = port;
    if (argc == 2)
    {
        std::cout << "Using port " << portno << std::endl;
        portno = atoi(argv[1]);
    }
    else if (argc > 2)
    {
        std::cout << "Too many arguments: exe [port]" << std::endl;
    }
    std::string local_IP = get_IP("en0");
    std::cout << "local_IP: " << local_IP << std::endl;

    mServerConnection con(portno);
    con.accept_connection_request();
    mFileSystem mfs;

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

    return 0;
}
