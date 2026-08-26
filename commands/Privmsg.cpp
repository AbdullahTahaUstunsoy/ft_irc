#include "Commands.hpp"

void Commands::Privmsg(Client& client, const std::vector<std::string>& params, Server& server)
{
    std::string srv_name = server.get_server_name();
    std::string nick = client.get_nickname().empty() ? "*" : client.get_nickname();

    if (params.size() < 2)
    {
        client.send_message(":" + srv_name + " 461 " + nick + " PRIVMSG :Not enough parameters", client.get_fd());
        return ;
    }
    if (!client.is_register())
    {
        client.send_message(":" + srv_name + " 451 " + nick + " :You have not registered", client.get_fd());
        return ;
    }
    else
    {
        if (params[0][0] != '#' && params[0][0] != '&')
        {
            if (server.get_client_fd(params[0]) == -1)
            {
                client.send_message(":" + srv_name + " 401 " + nick + " " + params[0] + " :No such nick", client.get_fd());
                return ;
            }
            client.send_message(":" + nick + " PRIVMSG " + params[0] + " :" + params[1], server.get_client_fd(params[0]));
        }
        else
        {
            Channel* channel = server.is_channel_exist(params[0]);
            if(!channel)
            {
                client.send_message(":" + srv_name + " 403 " + nick + " " + params[0] + " :No such channel", client.get_fd());
                return;
            }
            if(!channel->is_member(client.get_fd()))
            {
                client.send_message(":" + srv_name + " 404 " + nick + " " + params[0] + " :Cannot send to channel", client.get_fd());
                return;
            }
            channel->broadcast_message(":" + nick + " PRIVMSG " + params[0] + " :" + params[1] + + "\r\n", client.get_fd());
        }
    }
}