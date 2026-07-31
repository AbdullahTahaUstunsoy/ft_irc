#include "Commands.hpp"

void Commands::Nick(Client& client, const std::vector<std::string>& params, Server& server)
{
    if (params.empty())
    {
        client.send_message("empty param",client.get_fd());
        return ;
    }
    if (!client.get_pass())
    {
        client.send_message("pass false",client.get_fd());
        return ;
    }
    if (server.is_nick_unique(params[0]))
    {
        client.set_nick_name(params[0]);
        return ;
    }
    else
    {
        client.send_message("nick in use", client.get_fd());
        return ;
    }
}