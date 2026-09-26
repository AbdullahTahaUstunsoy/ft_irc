#include "Commands.hpp"

void Commands::Join(Client& client, const std::vector<std::string>& params, Server& server)
{
    std::string srv_name = server.get_server_name();
    std::string nick = client.get_nickname().empty() ? "*" : client.get_nickname();
    std::string prefix = client.get_prefix();
    Channel* target_channel;
    if (params.empty())
    {
        client.send_message(":" + srv_name + " 461 " + nick + " JOIN :Not enough parameters", client.get_fd());
        return ;
    }
    if (!client.is_register())
    {
        client.send_message(":" + srv_name + " 451 " + nick + " :You have not registered", client.get_fd());
        return ;
    }
    if (params[0][0] != '#' && params[0][0] != '&')
    {
        client.send_message(":" + srv_name + " 403 " + nick + " " + params[0] + " :No such channel", client.get_fd());
        return ;
    }
    target_channel = server.is_channel_exist(params[0]);
    if (target_channel)
    {
        if(target_channel->get_invite())
        {
            if(!target_channel->is_invited(client.get_fd()))
            {
                client.send_message(":" + srv_name + " 473 " + nick + " " + params[0] + " :Cannot join channel (+i)", client.get_fd());
                return ;
            }
        }
        if(target_channel->get_key())
        {
            if (params.size() < 2 || target_channel->get_password() != params[1])
            {
                client.send_message(":" + srv_name + " 475 " + nick + " " + params[0] + " :Cannot join channel", client.get_fd());
                return ;
            }
        }
        if (target_channel->get_member_limit() != -1)
        {
            if (static_cast<size_t>(target_channel->get_member_limit()) <= target_channel->get_members().size())
            {
                client.send_message(":" + srv_name + " 471 " + nick + " " + params[0] + " :Cannot join channel (+l)", client.get_fd());
                return ;
            }
        } 
        target_channel->add_member(&client);
        std::string join_msg = prefix + " JOIN :" + params[0];
        client.send_message(join_msg, client.get_fd());
        target_channel->broadcast_message(join_msg, client.get_fd(), 0);
        if (target_channel->get_topic().empty())
            client.send_message(":" + srv_name + " 331 " + nick + " " + params[0] + " :No topic is set", client.get_fd());
        else
            client.send_message(":" + srv_name + " 332 " + nick + " " + params[0] + " :" + target_channel->get_topic(), client.get_fd());
    }
    else
    {
        target_channel = server.create_channel(params[0], &client);
        std::string join_msg = prefix + " JOIN :" + params[0];
        client.send_message(join_msg, client.get_fd());
    }
    Commands::Names(client, params, server);
    Commands::Mode(client, params, server);
}