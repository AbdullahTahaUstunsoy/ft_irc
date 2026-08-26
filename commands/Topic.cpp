#include "Commands.hpp"

#include "Commands.hpp"

void Commands::Topic(Client& client, const std::vector<std::string>& params, Server& server)
{
    std::string srv_name = server.get_server_name();
    std::string nick = client.get_nickname().empty() ? "*" : client.get_nickname();

    if (params.empty())
    {
        client.send_message(":" + srv_name + " 461 " + nick + " TOPIC :Not enough parameters", client.get_fd());
        return ;
    }
    if (server.is_channel_exist(params[0]))
    {
        Channel *target_channel = server.get_channel(params[0]);
        if (target_channel->is_member(client.get_fd()))
        {
            if (params.size() == 1)
            {
                if (target_channel->get_topic().empty())
                {
                    client.send_message(":" + srv_name + " 331 " + nick + " " + params[0] + " :No topic is set", client.get_fd());
                    return ;
                }
                client.send_message(":" + srv_name + " 332 " + nick + " " + params[0] + " :" + target_channel->get_topic(), client.get_fd());
                return ;
            }
            else if ( params.size() == 2)
            {
                if(target_channel->get_topic_rest() && !target_channel->is_operator(client.get_fd()))
                {
                    client.send_message(":" + srv_name + " 482 " + nick + " " + params[0] + " :You're not channel operator", client.get_fd());
                    return ;
                }
                target_channel->set_topic(params[1]);
                target_channel->broadcast_message(":" + nick + " TOPIC " + params[0] + " :" + params[1] + "\r\n", -1);
                return ;
            }
        }
        client.send_message(":" + srv_name + " 442 " + nick + " " + params[0] + " :You're not on that channel", client.get_fd());
        return ;
    }
    client.send_message(":" + srv_name + " 403 " + nick + " " + params[0] + " :No such channel", client.get_fd());
    return ;
}