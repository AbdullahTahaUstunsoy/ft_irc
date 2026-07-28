#include "./Client.hpp"
#include "./Server.hpp"
#include <vector>
#include <iostream>

class Commands
{
    private:
        static std::string password;
    public:
        static void Pass(Client& client, const std::vector<std::string>& params, Server& server);
        static void Nick(Client& client, const std::vector<std::string>& params, Server& server);
        static void User(Client& client, const std::vector<std::string>& params, Server& server);
        static void Ping(Client& client, const std::vector<std::string>& params, Server& server);
};
