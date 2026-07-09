#include "Server.hpp"

int Server::getServerFd() const 
{
    return _serverFd;
}

void Server::setServerFd(int fd)
{
    _serverFd = fd;
}