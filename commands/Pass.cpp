#include "Commands.hpp"

void Commands::Pass(Client& client, const std::vector<std::string>& params, Server& server)
{
    if (params.empty())
    {
        client.send_message("empty param for pass", client.get_fd());
        return ;
    }
    if (client.is_register())
    {
        client.send_message("user aldready register", client.get_fd());
        return ;
    }
    if (params[0] != Commands::password)
    {
        client.send_message("password does not match", client.get_fd());
        return ;
    }
    client.set_pass();
}