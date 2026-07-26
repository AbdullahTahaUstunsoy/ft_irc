#include "Commands.hpp"

void Commands::execute(Client& client, const std::vector<std::string>& params, Server& server)
{
    if (params.empty())
    {
        std::cout << "empty param for pass" << std::endl;
        return ;
    }
    if (client.isregister())
    {
        //after client class are created 
        return ;
    }
    if (params[0] != Commands::password)
    {
        std::cout << "password does not match" << std::endl;
        return ;
    }
}