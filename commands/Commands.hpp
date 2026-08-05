#include "../Client.hpp"
#include "../Server.hpp"
#include <vector>
#include <iostream>
#include <sys/socket.h>
#include "../Channel.hpp"

class Commands
{
    private:
        static std::string password;
    public:
        static void Pass(Client& client, const std::vector<std::string>& params);
        static void Nick(Client& client, const std::vector<std::string>& params, Server& server);
        static void User(Client& client, const std::vector<std::string>& params);
        static void Ping(Client& client, const std::vector<std::string>& params);
        static void Privmsg(Client& client, const std::vector<std::string>& params, Server& server);
        static void Quit(Client& client, const std::vector<std::string>& params, Server& server);
        static void Join(Client& client, const std::vector<std::string>& params, Server& server);
};
