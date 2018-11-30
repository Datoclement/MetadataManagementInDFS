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
    const int inport = 4444;
    const int outport = 5555;
    const int numslaves = 1;

    int inportno = inport;
    int outportno = outport;

    // master should not change the port
    // slave of the same ip as master must change the port
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

    bool is_master = (local_IP == mastername) && (inportno == inport);

    if (!is_master)
    {
        int node_id = -1;
        std::cout << "I am a slave." << std::endl;
        std::cout << "I am going to work for the master " + mastername + " with his port " + std::to_string(inport) << std::endl;
        mClientConnection con(mastername, inport);
        std::string message(local_IP + std::to_string(inportno));
        while (true)
        {
            if (!con.writedown(message))
            {
                con = mClientConnection(mastername, inport);
                continue;
            }
            std::string line;
            if (!con.readin(line))
            {
                con = mClientConnection(mastername, inport);
                continue;
            }
            node_id = atoi(line.c_str());
            std::cout << "I am now registered as the " + std::to_string(node_id) + "-th slave of the master." << std::endl;
            break;
        }
        std::cout << "Ready to work!" << std::endl;
    }
    else
    {
        std::vector<std::vector<std::string> > slave_table;

        std::cout << "I am the mighty master." << std::endl;
        std::cout << "I am waiting for " << numslaves << " slaves to store the metadata." << std::endl;
        mServerConnection con(inportno);
        for (int i=0;i<numslaves;i++)
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
                slave_table.push_back(slave_profile);
                std::cout << "The " + std::to_string(i) + "-th slave is " + std::string(argv[0]) + " with its inport " + argv[1] << std::endl;
                break;
            }
        }
        std::cout << "So we can begin to work now!" << std::endl;
        // mServerConnection con(portno);
        // con.accept_connection_request();
        // mFileSystem mfs;
        //
        // while (true)
        // {
        //
        //     std::string line;
        //     std::vector<std::string> argv;
        //     std::string message;
        //
        //     if (!con.readin(line))
        //     {
        //         continue;
        //     }
        //
        //     tokenize(line, argv);
        //     mfs.run_command_line(argv, message);
        //     if (message.size() == 0)
        //     {
        //         message = "<no output>\n";
        //     }
        //     if (!con.writedown(message))
        //     {
        //         continue;
        //     }
        // }
    }
    return 0;
}
