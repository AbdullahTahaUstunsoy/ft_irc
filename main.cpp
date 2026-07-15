#include <iostream>
#include <cstring> //memset için, iostream içinde var ama garanti değil her derleyicinin kabul edeceği
#include <sys/socket.h> //SO_REUSEADDR, SOL_SOCKET, AF_INET, SOCK_STREAM gibi sabitler de bu header'da
#include <netinet/in.h>   // struct sockaddr_in, struct in_addr, INADDR_ANY
#include <unistd.h> //close için
#include <fcntl.h>  //fcntl(), F_SETFL, O_NONBLOCK için
#include <vector>
#include <poll.h> //poll() için
#include "Server.hpp"

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return (1);
    }
    //argümanların doğruluğunu kontrol edeceğim.
    int opt = 1;
    Server server;
    server.setServerFd(socket(AF_INET, SOCK_STREAM, 0)); //fd olusturulamayabilir diye kontrol yapmalı mıyım ? dinleme soketini kuruyorum. //serverFd değişkeni oluşturabilirim.
    setsockopt(server.getServerFd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); //int dönüyor bunu kontrol etmeli miyim ? //sunucuyu kapatığ açtığımızda portun hala kullanımda olmasını önlemek için gerekiyor.

    struct sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr)); //addr değişkenini sıfırlıyorum. üstüne yazacağız ama struct'ın son değişkeni sıfırlanmıyor onu da sıfırlamak için memset kullandım. 

    addr.sin_family = AF_INET; //IPv4
    addr.sin_addr.s_addr = INADDR_ANY; //tüm IP adreslerinden gelen bağlantıları kabul et.
    addr.sin_port = htons(std::atoi(argv[1]));

    if(bind(server.getServerFd(), (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        std::cerr << "Error binding socket" << std::endl;
        close(server.getServerFd());   // ← açtığın fd'yi kapat
        return (1); //exception fırlatabilirim.
    }

    if(listen(server.getServerFd(), SOMAXCONN) < 0)
    {
        std::cerr << "Error listening on socket" << std::endl;
        close(server.getServerFd());   // ← açtığın fd'yi kapat
        return (1);
    }

    if(fcntl(server.getServerFd(), F_SETFL, O_NONBLOCK) < 0)
    {
        std::cerr << "Error setting socket to non-blocking" << std::endl;
        close(server.getServerFd());
        return (1);
    }

    std::vector<struct pollfd> pollFds;
    struct pollfd serverPollFd;
    serverPollFd.fd = server.getServerFd();
    serverPollFd.events = POLLIN;
    serverPollFd.revents = 0;
    pollFds.push_back(serverPollFd);

    poll(&pollFds[0], pollFds.size(), -1);
}