#include "Server.hpp"
#include "Client.hpp"

Server::Server(int port, const std::string& password) : _serverFd(-1) , _portNum(port) , _password(password), _running(true)
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

void Server::removeFds()
{
    //placeholder
}
void Server::acceptClients()
{
    std::cout << "POLLIN on server socket" << std::endl;
}
void Server::handleClients(int fd)
{
    (void)fd;
    //placeholder
}

void Server::runServer() //Reactor Pattern
{
    while(_running)
    {
        poll(&_pollFds[0], _pollFds.size(), -1); //dönüş değerini kontrol etmeli miyim ? dönüş değeri kaç tane file descriptor'da olay (event) gerçekleştiğini söyler. //Ctrl+C geldiğinde poll sinyal yüzünden -1 ile kesiliyor.
        for(size_t i = 0; i < _pollFds.size(); i++)
        {
            short revents = _pollFds[i].revents; //pollfd struct'ındaki revents short tipinde.
            if(revents == 0) //revents 0 ise bu fd'de bir olay yok demektir. Bu yüzden döngüye devam ediyorum.
                continue;
            if(_pollFds[i].fd == _serverFd) //serverfd
            {    
                if(revents & POLLIN) //POLLERR kontrolü gerekli mi buraya ?          
                    acceptClients();
                continue;
            }
            //clientfd
            if(revents & POLLIN)
                handleClients(_pollFds[i].fd);
            if(revents & (POLLHUP | POLLERR)) //revents'te POLLIN | POLLHUP durumu olabilir o yüzden if mantıklı diye düşündüm. // her iki durumda da fd'yi kapatacağız dolayısıyla aynı if'te değerlendirebiliriz
                removableFds.insert(_pollFds[i].fd);
        }
        removeFds(); //removableFds'deki fd'leri kapatıp _pollFds'den sileceğiz.
    }
}