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
void Server::acceptClients() //burada gerçek accept olmayıp mesaj olduğunda kuyruk azalmıyor ve sürekli mesaj basıyor
{
    int clientFd = accept(_serverFd, NULL, NULL);
    if (clientFd < 0) //bir client'ın kabul edilememesi, sunucunun ölmesini gerektirmez. Kurulum hatalarından farkı budur.
        return;
    if (fcntl(clientFd, F_SETFL, O_NONBLOCK) < 0)
    {
        close(clientFd);
        return;
    }
    addToPoll(clientFd);
    _clients[clientFd] = new Client(clientFd); //Bu, o client'ın buffer'ına ulaşmamızı sağlıyor.
} //Sonra duruma göre kuyruktaki herkesi döngüyle accept edebilirim. Şuan bir accept oluyor ve bir sonraki poll'da diğer accept oluyor  


void Server::handleClients(int fd)
{
    char buf[1024]; //RFC'ye göre bir IRC mesajı en fazla 512 byte (\r\n dahil), buf boyutunu değiştirebilirim.
    ssize_t n = recv(fd, buf, sizeof(buf), 0);
    if(n<=0)
    {
        removableFds.insert(fd);
        return;
    }
    std::map<int, Client*>::iterator it = _clients.find(fd); //iteratore geçtim çünkü [] kullanımı, olmayan anahtarı oluşturuyor. Yani fd mapte yoksa kendi oluşturup olmayan fonksiyona erişmeye çalışacak bu durumda da hata alırız.
    if(it == _clients.end())
        return;
    Client* client = it->second;
    client->add_buffer(buf,n);
    std::string line;
    while (client->line_end_check(line))
    {
        std::cout << "[" << fd << "] " << line << std::endl; //debug için, sileceğim
    }
}

void Server::runServer() //Reactor Pattern
{
    while(_running)
    {
        int eventCount = poll(&_pollFds[0], _pollFds.size(), -1); //dönüş değerini kontrol etmeli miyim ? dönüş değeri kaç tane file descriptor'da olay (event) gerçekleştiğini söyler. //Ctrl+C geldiğinde poll sinyal yüzünden -1 ile kesiliyor.
        if(eventCount < 0) //bu durumda poll gerçekten başarısız olmuş olabilir veya SIGINT (CTRL + C) sinyali gelmiş olabilir. 
        {
            if(!_running) //bu durumda SIGINT sinyali gelmiş demektir. (CTRL + C). Henüz SIGINT handler yazmadım şu an idle duruyor
                break; //SIGINT sinyali ile kesildiğinde exception fırlatılmasına gerek yok çünkü bu zaten kullanıcı programı kapatmak istiyor
            throw std::runtime_error("poll failed");  
        }
        for(size_t i = 0; i < _pollFds.size(); i++)
        {
            short revents = _pollFds[i].revents; //pollfd struct'ındaki revents short tipinde.
            if(revents == 0) //revents 0 ise bu fd'de bir olay yok demektir. Bu yüzden döngüye devam ediyorum.
                continue;
            //POLLERR POLLNVAL bakılabilir
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

// for commands function

bool Server::is_nick_unique(std::string nick)
{
    std::map<int, Client*>::iterator it;
    it = _clients.begin();

    for (it; it != _clients.end(); it++)
    {
        if (it->second->get_nickname() == nick)
            return (false);
    }
    return (true);
}