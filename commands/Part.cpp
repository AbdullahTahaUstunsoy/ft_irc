#include "Commands.hpp"

void Commands::Part(Client& client, const std::vector<std::string>& params, Server& server)
{
    if (params.empty())
    {
        client.send_message("empty parameter", client.get_fd());
        return ;
    }
    if (server.is_channel_exist(params[0]))
    {
        Channel *target_channel = server.get_channel(params[0]);
        if (target_channel->is_member(client.get_fd()))
        {
            if (params.size() == 2)
                target_channel->left_channel(client.get_fd(), params[1]);   
            else
                target_channel->left_channel(client.get_fd());
            if(target_channel->get_members().empty())
                server.delete_channel(params[0]);
            return ;
        }
        client.send_message("you are not a member", client.get_fd());
        return ;
    }
    client.send_message("no channel with this name", client.get_fd());
    return ;
}