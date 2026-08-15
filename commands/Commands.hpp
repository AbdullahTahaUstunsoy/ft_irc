#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "../Client.hpp"
#include "../Server.hpp"
#include <vector>
#include <iostream>
#include <sys/socket.h>
#include "../Channel.hpp"

class Server;

class Commands
{
    private:
        // static std::string password;
    public: 
        static void run_command(Client& client, const std::string& command, const std::vector<std::string>& params, Server& server);
        static void Pass(Client& client, const std::vector<std::string>& params, Server& server);
        static void Nick(Client& client, const std::vector<std::string>& params, Server& server);
        static void User(Client& client, const std::vector<std::string>& params);
        static void Ping(Client& client, const std::vector<std::string>& params);
        static void Privmsg(Client& client, const std::vector<std::string>& params, Server& server);
        static void Quit(Client& client, const std::vector<std::string>& params, Server& server);
        static void Join(Client& client, const std::vector<std::string>& params, Server& server);
        static void Kick(Client &client, const std::vector<std::string> &params, Server &server);
        static void Invite(Client &client, const std::vector<std::string> &params, Server &server);
        static void Part(Client& client, const std::vector<std::string>& params, Server& server);
        static void Topic(Client& client, const std::vector<std::string>& params, Server& server);
};

#endif
