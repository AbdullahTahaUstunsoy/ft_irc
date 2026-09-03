#include "Commands.hpp"

void Commands::Join(Client& client, const std::vector<std::string>& params, Server& server)
{
    std::string srv_name = server.get_server_name();
    std::string nick = client.get_nickname().empty() ? "*" : client.get_nickname();
    std::string prefix = ":" + nick + "!" + client.get_username() + "@127.0.0.1";
    Channel* target_channel;
    if (params.empty())
    {
        std::cout << "Burada 1" << std::endl;
        client.send_message(":" + srv_name + " 461 " + nick + " JOIN :Not enough parameters", client.get_fd());
        return ;
    }
    if (!client.is_register())
    {
        std::cout << "Burada 2" << std::endl;
        client.send_message(":" + srv_name + " 451 " + nick + " :You have not registered", client.get_fd());
        return ;
    }
    if (params[0][0] != '#' && params[0][0] != '&')
    {
        std::cout << "Burada 3" << std::endl;
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
                std::cout << "Burada 4" << std::endl;
                return ;
            }
            if(target_channel->get_key())
            {
                std::cout << "Burada 5" << std::endl;
                if (target_channel->get_password() != params[1]) // potential segfault if the user wants to join a channel that have a password
                {
                    std::cout << "Burada 6" << std::endl;
                    client.send_message(":" + srv_name + " 475 " + nick + " " + params[0] + " :Cannot join channel (+k)", client.get_fd());
                    return ;
                }
            }
            if (target_channel->get_member_limit() != -1)
            {
                std::cout << "Burada 7" << std::endl;
                if (static_cast<size_t>(target_channel->get_member_limit()) <= target_channel->get_members().size())
                {
                    std::cout << "Burada 8" << std::endl;
                    client.send_message(":" + srv_name + " 471 " + nick + " " + params[0] + " :Cannot join channel (+l)", client.get_fd());
                    return ;
                }
            } 
        }
        target_channel->add_member(&client);
        std::string join_msg = prefix + " JOIN :" + params[0];
        client.send_message(join_msg, client.get_fd());
        target_channel->broadcast_message(join_msg, client.get_fd());
    }
    else
    {
    target_channel = server.create_channel(params[0], &client);
    std::string join_msg = prefix + " JOIN :" + params[0];
    client.send_message(join_msg, client.get_fd());
    }
}