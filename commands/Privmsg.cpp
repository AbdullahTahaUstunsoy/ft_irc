#include "Commands.hpp"

void Commands::Privmsg(Client& client, const std::vector<std::string>& params, Server& server)
{
    if (params.size() < 2)
    {
        client.send_message("params error\r\n", client.get_fd());
        return ;
    }
    if (!client.is_register())
    {
        client.send_message("should register\r\n", client.get_fd());
        return ;
    }
    else
    {
        if (params[0][0] != '#' && params[0][0] != '&')
        {
            if (server.get_client_fd(params[0]) == -1)
            {
                client.send_message("there is no user with this nick\r\n", client.get_fd());
                return ;
            }
            client.send_message(params[1], server.get_client_fd(params[0]));
        }
        else
        {
            Channel* channel = server.is_channel_exist(params[0]);
            if(!channel)
            {
                client.send_message("There is no such channel", server.get_client_fd(params[0]));
                return;
            }
            if(!channel->is_member(client.get_fd()))
            {
                client.send_message("You are not member of this channel", server.get_client_fd(params[0]));
                return;
            }
            channel->broadcast_message(params[1],client.get_fd());
        }

    }
}