#include "Server.hpp"
#include "Client.hpp"
#include <csignal>
#include <cerrno>

volatile sig_atomic_t g_running = 1;

Server::Server(int port, const std::string& password) : _serverFd(-1) , _portNum(port) , _password(password)
{
}

Server::~Server()
{
    for(std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        close(it->first);
        delete (it->second);
    }
    _clients.clear();
    if (_serverFd >= 0)
        close(_serverFd);
}

sockaddr_in Server::configureSockAddrIn(int _portNum)
{
    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(_portNum);

    return addr;
}

void Server::configureServerSocket()
{
    _serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverFd < 0)
        throw std::runtime_error("socket failed");
    int opt = 1;
    if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        throw std::runtime_error("setsockopt failed");
    sockaddr_in addr = configureSockAddrIn(_portNum);
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

void Server::removeFromPoll(int fd)
{
    for (std::vector<struct pollfd>::iterator pit = _pollFds.begin(); pit != _pollFds.end(); pit++)
    {
        if (pit->fd == fd) 
        {
            _pollFds.erase(pit);
            break;
        }
    }
}

void Server::removeClient(int fd)
{
    std::map<int, Client*>::iterator cit = _clients.find(fd);
    if(cit != _clients.end())
    {
        delete(cit->second);
        _clients.erase(cit);
    }
}

void Server::removeFds()
{
    for(std::set<int>::iterator sit = removableFds.begin(); sit != removableFds.end(); sit++)
    {
        int fd = *sit;
        if(fd == _serverFd)
            continue;
        removeFromPoll(fd);
        removeClient(fd);
        close(fd);
    }
    removableFds.clear();
}

void Server::acceptClients() //burası döngüye alınabilir
{
    int clientFd = accept(_serverFd, NULL, NULL);
    if (clientFd < 0)
        return;
    if (fcntl(clientFd, F_SETFL, O_NONBLOCK) < 0)
    {
        close(clientFd);
        return;
    }
    addToPoll(clientFd);
    _clients[clientFd] = new Client(clientFd);
} 

void Server::removeCRLF(std::string& line)
{
    if(!line.empty() && line[line.size() - 1] == '\n')
        line.erase(line.size() - 1);
    if(!line.empty() && line[line.size() - 1] == '\r')
        line.erase(line.size() - 1);
}

void Server::handleClients(int fd)
{
    char buf[1024];
    ssize_t n = recv(fd, buf, sizeof(buf), 0);
    if(n <= 0)
    {
        removableFds.insert(fd);
        return;
    }
    std::map<int, Client*>::iterator it = _clients.find(fd);
    if(it == _clients.end())
        return;
    Client* client = it->second;
    client->add_buffer(buf,n);
    std::string line;
    while (client->line_end_check(line))
        removeCRLF(line);   
}

void handleSigint(int signum)
{
    (void)signum;
    g_running = 0;
}

void Server::handlePollEvents()
{
    for(size_t i = 0; i < _pollFds.size(); i++)
    {
        short revents = _pollFds[i].revents;
        if(revents == 0)
            continue;
        if(_pollFds[i].fd == _serverFd)
        {
            if(revents & POLLIN)
                acceptClients();
            continue;
        }
        if(revents & POLLIN)
            handleClients(_pollFds[i].fd);
        if(revents & (POLLHUP | POLLERR))
            removableFds.insert(_pollFds[i].fd);
    }
}

void Server::runServer() //Reactor Pattern
{
    signal(SIGINT, handleSigint);
    signal(SIGPIPE, SIG_IGN);
    while(g_running)
    {
        if(poll(&_pollFds[0], _pollFds.size(), -1) < 0) 
        {
            if(!g_running)
                break;
            throw std::runtime_error("poll failed");  
        }
        handlePollEvents();
        removeFds();
    }
}

void Server::sendToClient(int fd, const std::string& msg) //errno'ya bakmaya gerek var mı gerçekten EAGAIN ECONNRESET
{
    std::string message = msg + "\r\n";
    ssize_t rval = send(fd, message.c_str(), message.size(), 0);
    if (rval < 0)
    {
        if (errno == EPIPE || errno == ECONNRESET) //EAGAIN EWOULDBACK < 0 ama fd'nin kaldırılmasını gerektirmiyor
            removableFds.insert(fd);
    }
}

// for commands function

bool Server::is_nick_unique(std::string nick)
{
    std::map<int, Client*>::iterator it;
    it = _clients.begin();

    for (it = _clients.begin(); it != _clients.end(); it++)
    {
        if (it->second->get_nickname() == nick)
            return (false);
    }
    return (true);
}

int Server::get_client_fd(std::string nick)
{
    std::map<int, Client*>::iterator it;

    for (it = _clients.begin(); it != _clients.end(); it++)
    {
        if (it->second->get_nickname() == nick)
            return (it->first);
    }
    return (-1);
}

void Server::quit_util(int fd, std::string message)
{
    //removeClient
}

Channel* Server::is_channel_exist(std::string channel_name)
{
    std::map<std::string, Channel*>::iterator it;

    for (it = channels.begin(); it != channels.end(); it++)
    {
        if (it->first == channel_name)
            return (it->second);
    }
    return (NULL);
}

Channel* Server::create_channel(std::string name, Client* client)
{
    Channel* new_channel = new Channel(name);
    channels[name] = new_channel;
    new_channel->add_member(client);
    new_channel->add_operator(client->get_fd());
    return (new_channel);
}