#include "Commands.hpp"

void Commands::Nick(Client& client, const std::vector<std::string>& params, Server& server)
{
    if (params.empty())
    {
        std::cout << "empty param for nick" << std::endl;
        return ;
    }
    if (is_nick_unique(params[0]))
    {
        set_nick_name(params[0]);
        return ;
    }
    else
    {
        std::cout << "nick in use" << std::endl;
        return ;
    }
}