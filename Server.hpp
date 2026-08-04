#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <poll.h>
#include <sys/socket.h>
#include <stdexcept>
#include <cstring>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include <cstdlib>
#include <set>
#include <map>

class Server {
    private:
        int _serverFd;
        int _portNum;
        std::string _password;
        std::vector<struct pollfd> _pollFds;
        void addToPoll(int fd);

        std::set<int> removableFds;
        sockaddr_in configureSockAddrIn(int portNum);
        void removeFds();
        void removeFromPoll(int fd);
        void removeClient(int fd);
        void removeCRLF(std::string& line);
        void handlePollEvents();


        void acceptClients();
        void handleClients(int fd);
        std::map<int, Client*> _clients;
        
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
        void sendToClient(int fd, const std::string& msg);
};

#endif