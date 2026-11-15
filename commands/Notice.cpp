#include "Commands.hpp"

void Commands::Notice(Client& client, const std::vector<std::string>& params, Server& server)
{
    std::string srv_name = server.get_server_name();
    std::string nick = client.get_nickname().empty() ? "*" : client.get_nickname();

    if (params.size() < 2)
        return ;
    if (!client.is_register())
        return ;
    else
    {
        if (params[0][0] != '#' && params[0][0] != '&')
        {
            if (server.get_client_fd(params[0]) == -1)
                return ;
            client.send_message(client.get_prefix() + " NOTICE " + params[0] + " :" + params[1], server.get_client_fd(params[0]));
        }
        else
        {
            Channel* channel = server.is_channel_exist(params[0]);
            if(!channel)
                return;
            if(!channel->is_member(client.get_fd()))
                return;
            channel->broadcast_message(client.get_prefix() + " NOTICE " + params[0] + " :" + params[1], client.get_fd(), 0);
        }
    }
}