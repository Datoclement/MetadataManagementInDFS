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

#define N 512

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    int portno = atoi(argv[1]);

    mServerConnection con(portno);
    con.accept_connection_request();
    mFileSystem mfs;

    while (true)
    {
        // bzero(buffer,256);
        // n = read(newsockfd,buffer,255);
        // if (n <= 0)
        // {
        //     std::cout << "socket connection stopped." << std::endl;
        //     newsockfd = con.accept_connection_request();
        //     continue;
        // }
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
