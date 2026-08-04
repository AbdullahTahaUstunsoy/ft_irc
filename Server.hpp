#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include <iostream>
#include <unistd.h>//close için
#include <fcntl.h>//fcntl(), F_SETFL, O_NONBLOCK için
#include <vector>
#include <poll.h>
#include <sys/socket.h>
#include <stdexcept>//std::runtime_error, std::logic_error vb. //kendi exception class'ımı oluşturabilirim
#include <cstring>
#include <netinet/in.h> //struct sockaddr_in, struct in_addr, INADDR_ANY
#include <string>
#include <arpa/inet.h> //htons
#include <cstdlib>
#include <set>
#include <map>

class Server {
    private:
        int _serverFd;
        int _portNum;
        std::string _password;
        bool _running;
        std::vector<struct pollfd> _pollFds;
        void addToPoll(int fd);

        std::set<int> removableFds; //Kapatılacak fd'leri tutacağım. (POLLHUP ve POLLERR durumları için) //set yaptım çünkü aynı fd'yi birden fazla kez eklememek için.
        void removeFds(); //removableFds'deki fd'leri kapatıp _pollFds'den sileceğiz.
        void acceptClients();
        void handleClients(int fd);
        std::map<int, Client*> _clients;   // fd → Client
        //copy consturcor ve copy assignment'ı biz yazmasak bile derleyici otomatik yazdığı için private olarak tanımladım. yoksa sorun olabilir. (double fd close vs.)
        Server(const Server&);
        Server& operator=(const Server&);
    public:
        void quit_util(int fd, std::string message);
        int get_client_fd(std::string nick);
        bool is_nick_unique(std::string nick);
        Server (int port, const std::string& password);
        ~Server();
        void configureServerSocket();
        void runServer();
};




#endif