#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <iostream>
#include "mconnection.hpp"
#include "merror.hpp"

mServerConnection::mServerConnection(int portno)
{
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->sockfd < 0)
    {
        error("ERROR opening socket");
    }

    bzero((char *) &(this->serv_addr), sizeof(this->serv_addr));
    this->portno = portno;
    this->serv_addr.sin_family = AF_INET;
    this->serv_addr.sin_addr.s_addr = INADDR_ANY;
    this->serv_addr.sin_port = htons(portno);
    if (bind(this->sockfd, (struct sockaddr *) &(this->serv_addr), sizeof(this->serv_addr)) < 0)
    {
        error("ERROR on binding");
    }
    listen(this->sockfd, 5);
    this->clilen = sizeof(this->cli_addr);
    this->client_addr = (struct sockaddr *) &(this->cli_addr);
}

int mServerConnection::accept_connection_request()
{
    this->newsockfd = accept(this->sockfd, this->client_addr, &(this->clilen));
    if (this->newsockfd < 0)
    {
        error("ERROR on accept");
    }
    return this->newsockfd;
}

bool mServerConnection::readin(std::string& line)
{
    char buffer[mconnection_bandwidth];
    bzero(buffer,mconnection_bandwidth);
    int n = read(this->newsockfd,buffer,mconnection_bandwidth-1);
    if (n <= 0)
    {
        std::cout << "socket connection stopped." << std::endl;
        this->accept_connection_request();
        return false;
    }
    line = std::string(buffer);
    return true;
}

bool mServerConnection::writedown(const std::string& message)
{
    int n = write(this->newsockfd, message.c_str(), message.size());
    if (n <= 0)
    {
        std::cout << "socket connection stopped." << std::endl;
        this->accept_connection_request();
        return false;
    }
    return true;
}

mServerConnection::~mServerConnection()
{
    close(this->newsockfd);
    close(this->sockfd);
}

mClientConnection::mClientConnection(const std::string& hostname, int portno)
{
    this->portno = portno;
    this->server = gethostbyname(hostname.c_str());
    if (this->server == nullptr)
    {
        std::cout << "Error: fails to get the hostname." << std::endl;
        return;
    }
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->sockfd < 0)
    {
        std::cout << "Error: fails to open the socket." << std::endl;
        return;
    }
    bzero((char *) &this->serv_addr, sizeof(this->serv_addr));
    this->serv_addr.sin_family = AF_INET;
    bcopy((char *)this->server->h_addr,
        (char *)&this->serv_addr.sin_addr.s_addr,
        this->server->h_length);
    this->serv_addr.sin_port = htons(this->portno);
    if (connect(this->sockfd,(struct sockaddr *) &(this->serv_addr),sizeof(this->serv_addr)) < 0)
    {
        std::cout << "Error: connection fails." << std::endl;
        return;
    }
}

bool mClientConnection::readin(std::string& line)
{
    char buffer[mconnection_bandwidth];
    bzero(buffer,mconnection_bandwidth);
    int n = read(this->sockfd,buffer,mconnection_bandwidth-1);
    if (n < 0)
    {
        std::cout << "Error: read operation non succeeded." << std::endl;
        return false;
    }
    line = std::string(buffer);
    return true;
}

bool mClientConnection::writedown(const std::string& message)
{
    int n = write(this->sockfd,message.c_str(),message.size());
    if (n < 0)
    {
        std::cout << "Error: message writing non succeeded." << std::endl;
        return false;
    }
    return true;
}

mClientConnection::~mClientConnection()
{
    close(this->sockfd);
}

int mClientConnection::get_sockfd()
{
    return this->sockfd;
}
