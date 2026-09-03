#include "Commands.hpp"

void Commands::Part(Client& client, const std::vector<std::string>& params, Server& server)
{
    std::string srv_name = server.get_server_name();
    std::string nick = client.get_nickname().empty() ? "*" : client.get_nickname();

    if (params.empty())
    {
        client.send_message(":" + srv_name + " 461 " + nick + " PART :Not enough parameters", client.get_fd());
        return ;
    }
    if (server.is_channel_exist(params[0]))
    {
        Channel *target_channel = server.get_channel(params[0]);
        std::cout << "PART a gelen channel adi" + params[0] << std::endl;
        if (target_channel->is_member(client.get_fd()))
        {
            if (params.size() == 2)
            {
                target_channel->left_channel(client.get_fd(), params[1]);   
            }
            else
                target_channel->left_channel(client.get_fd());
            if(target_channel->get_members().empty())
                server.delete_channel(params[0]);
            return ;
        }
        client.send_message(":" + srv_name + " 442 " + nick + " " + params[0] + " :You're not on that channel", client.get_fd());
        return ;
    }
    client.send_message(":" + srv_name + " 403 " + nick + " " + params[0] + " :No such channel", client.get_fd());
    return ;
}