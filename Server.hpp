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
#include <arpa/inet.h>
#include <cstdlib>
#include <set>
#include <map>
#include "Channel.hpp"
#include "Parser.hpp"
#include "commands/Commands.hpp"

class Server {
    private:
        int _serverFd;
        int _portNum;
        std::string _serverName;
        std::string _password;
        std::vector<struct pollfd> _pollFds;
        void addToPoll(int fd);

        std::set<int> removableFds;
        sockaddr_in configureSockAddrIn() const;
        void removeFds();
        void removeFromPoll(int fd);
        void removeClient(int fd);
        void handlePollEvents();


        void acceptClients();
        void handleClients(int fd);
        std::map<int, Client*> _clients;
        

        Server(const Server&);
        Server& operator=(const Server&);

        std::map<std::string, Channel*> channels;
    public:
        const std::string& get_server_name();
        void quit_util(Client& client, std::string message);
        int get_client_fd(std::string nick);
        bool is_nick_unique(std::string nick);
        Channel* create_channel(std::string name, Client* client);
        Channel* is_channel_exist(std::string channel_name);
        Server (int port, const std::string& password);
        ~Server();
        void configureServerSocket();
        void runServer();
        void sendToClient(int fd, const std::string& msg);
        Channel* get_channel(std::string name);
        void delete_channel(std::string channel_name);

        const std::string& getPassword() const;
		Client* getClient(int fd);  
};

#endif