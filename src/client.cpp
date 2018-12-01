#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <iostream>
#include <string>
#include "api.hpp"
#include "merror.hpp"
#include "mconnection.hpp"

#define N 512

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        error("Error usage %s hostname port");
    }

    string hostname = string(argv[1]);
    int portno = atoi(argv[2]);
    mClientConnection con(hostname, portno);
    int sockfd = con.get_sockfd();
    char buffer[N];
    while (true)
    {
        printf("$ ");
        bzero(buffer,N);
        fgets(buffer,N-1,stdin);
        string line = string(buffer);
        vector<string> command;
        tokenize(line, command);
        if (command.size() == 0)
        {
            continue;
        }
        else
        {
            if (command[0] == "exit" or command[0] == "quit")
            {
                break;
            }
            else
            {
                string message(buffer);
                if (!con.writedown(message))
                {
                    con = mClientConnection(hostname, portno);
                    continue;
                }
                string line;
                if (!con.readin(line))
                {
                    con = mClientConnection(hostname, portno);
                    continue;
                }
                if (line != "<no output>\n")
                {
                    cout << line;
                }
            }
        }
    }
    return 0;
}
