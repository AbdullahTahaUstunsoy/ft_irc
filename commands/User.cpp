#include "Commands.hpp"

void Commands::User(Client& client, const std::vector<std::string>& params, Server& server)
{
    if (params.size() != 4)
    {
        std::cout << "should be 4 parameter" << std::endl;
        return ;
    }
    if (is_regitster(client))
    {
        std::cout << "user was already register" << std::endl;
        return ;
    }
    set_user_name(params[3]);
}
