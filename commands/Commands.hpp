#include "./Client.hpp"
#include "./Server.hpp"
#include <vector>
#include <iostream>

class Commands
{
    private:
        static std::string password;
    public:
        static void execute(Client& client, const std::vector<std::string>& params, Server& server);
};