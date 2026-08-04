#include "Commands.hpp"

void Commands::Privmsg(Client& client, const std::vector<std::string>& params, Server& server)
{
    if (params.size() != 2)
    {
        client.send_message("params error", client.get_fd());
        return ;
    }
    if (!client.is_register())
    {
        client.send_message("should register", client.get_fd());
        return ;
    }
    else
    {
        if (server.get_client_fd(params[0]) == -1)
        {
            client.send_message("there is no user with this nick", client.get_fd());
            return ;
        }
        client.send_message(params[1], server.get_client_fd(params[0]));
    }
}