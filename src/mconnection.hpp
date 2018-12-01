#ifndef MCONNECTION_HPP
#define MCONNECTION_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

const int mconnection_bandwidth = 512;

class mServerConnection
{
public:
    mServerConnection(int portno);
    int accept_connection_request();
    bool readin(std::string& line);
    bool writedown(const std::string& message);
    std::string get_client_ip_addr();
    ~mServerConnection();

private:
    int sockfd;
    int portno;
    int newsockfd;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    struct sockaddr * client_addr;
    std::string client_ip_address_str;
};

class mClientConnection
{
public:
    mClientConnection(const std::string& hostname, int portno);
    bool readin(std::string& line);
    bool writedown(const std::string& message);
    ~mClientConnection();
    int get_sockfd();

private:
    int sockfd;
    int portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;
};

#endif
