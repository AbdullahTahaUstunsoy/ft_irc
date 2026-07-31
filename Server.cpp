#include "Server.hpp"
#include "Client.hpp"

Server::Server(int port, const std::string& password) : _serverFd(-1) , _portNum(port) , _password(password), _running(true)
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
    for(std::set<int>::iterator sit = removableFds.begin(); sit != removableFds.end(); sit++) //set'e erişimin tek yolu iterator. [] overload set'te yok.
    {
        int fd = *sit;
        if(fd == _serverFd)
            continue;
        for (std::vector<struct pollfd>::iterator pit = _pollFds.begin(); pit != _pollFds.end(); pit++)
        {
            if (pit->fd == fd) 
            {
                _pollFds.erase(pit);
                break;
            }
        }

        std::map<int, Client*>::iterator cit = _clients.find(fd);
        if(cit != _clients.end())
        {
            delete(cit->second);
            _clients.erase(cit);
        }
        close(fd);
    }
    removableFds.clear();
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
} 
//Sonra duruma göre kuyruktaki herkesi döngüyle accept edebilirim. Şuan bir accept oluyor ve bir sonraki poll'da diğer accept oluyor  


void Server::handleClients(int fd)
{
    char buf[1024]; //RFC'ye göre bir IRC mesajı en fazla 512 byte (\r\n dahil), buf boyutunu değiştirebilirim.
    ssize_t n = recv(fd, buf, sizeof(buf), 0);
    if(n <= 0)
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
        size_t size = line.size();
        if(!line.empty() && line[size-1] == '\n')
            line.erase(size-1);
        if(!line.empty() && line[size-1] == '\r')
            line.erase(size-1);
        std::cout << "[" << fd << "] " << line << std::endl; //debug için, sileceğim
        //sendToClients(fd, "ECHO: " + line); //debug içindi
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

    /*
    client ctrl+c yaptıktan sonra EOF olur ve if(revents & POLLIN) bloğundan handleClients'a gidilir.
    burada da removableFds.insert(fd) olur ve return olur. ardından alttaki if bloğuna girilip burada fd tekrar insert edilmeye
    çalışılabilir fakat biz removableFds bir set container'ı olduğu için tekrar insert edilmeye çalışılırsa bu işlem yok sayılır.
    Bununla beraber Linux'ta normal client çıkışında (nc ctrl+c) revents çoğunlukla sadece POLLIN olur — POLLHUP set edilmiyor. Yani ikinci if çoğu zaman tetiklenmiyor bile.
    POLLHUP daha çok anormal kopmalarda geliyor. Bizim recv == 0 yolumuz asıl mekanizma, POLLHUP kontrolü yedek güvence.
    */
}

void Server::sendToClients(int fd, const std::string& msg){
    std::string message = msg + "\r\n";
    ssize_t rval = send(fd, message.c_str(), message.size(), 0); //server'dam tek bir client'a veri göndermek için.
    if(rval < 0)
        removableFds.insert(fd);
}