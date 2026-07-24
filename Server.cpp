#include "Server.hpp"

Server::Server(int port, const std::string& password) : _serverFd(-1) , _portNum(port) , _password(password)
{
}

Server::~Server()
{
    if (_serverFd >= 0)
        close(_serverFd);
}

void Server::configureServerSocket()
{
    _serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverFd < 0)
        throw std::runtime_error("socket failed");
    int opt = 1;
    if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        throw std::runtime_error("setsockopt failed");
    struct sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr)); 
    addr.sin_family = AF_INET; //IPv4
    addr.sin_addr.s_addr = INADDR_ANY; //tüm IP adreslerinden gelen bağlantıları kabul et.
    addr.sin_port = htons(_portNum);

    
    if(bind(_serverFd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
        throw std::runtime_error("bind failed");

    if(listen(_serverFd, SOMAXCONN) < 0)
        throw std::runtime_error("listen failed");

    if(fcntl(_serverFd, F_SETFL, O_NONBLOCK) < 0)
        throw std::runtime_error("fcntl failed");
    addToPoll(_serverFd);
}

void Server::addToPoll(int fd)
{
    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLIN;
    pfd.revents = 0;
    _pollFds.push_back(pfd);
}

void Server::runServer()
{   
}