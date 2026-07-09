#include <iostream>
#include <cstring> //memset için, iostream içinde var ama garanti değil her derleyicinin kabul edeceği
#include <sys/socket.h> //SO_REUSEADDR, SOL_SOCKET, AF_INET, SOCK_STREAM gibi sabitler de bu header'da
#include <netinet/in.h>   // struct sockaddr_in, struct in_addr, INADDR_ANY
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
    server.setServerFd(socket(AF_INET, SOCK_STREAM, 0)); //dinleme soketini kuruyorum. //serverFd değişkeni oluşturabilirim.
    setsockopt(server.getServerFd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); //sunucuyu kapatığ açtığımızda portun hala kullanımda olmasını önlemek için gerekiyor.

    struct sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr)); //addr değişkenini sıfırlıyorum. üstüne yazacağız ama struct'ın son değişkeni sıfırlanmıyor onu da sıfırlamak için memset kullandım. 

}   