#include "Commands.hpp"

void Commands::Join(Client& client, const std::vector<std::string>& params, Server& server)
{
    Channel* target_channel;
    if (params.empty())
    {
        client.send_message("Not enough parameters", client.get_fd());
        return ;
    }
    if (!client.is_register())
    {
        client.send_message("You have not registered", client.get_fd());
        return ;
    }
    if (params[0][0] != '#' && params[0][0] != '&')
    {
        client.send_message(params[0] + "No such channel", client.get_fd());
        return ;
    }
    target_channel = server.is_channel_exist(params[0]);
    if (target_channel)
        target_channel->add_member(&client);
    else
    {
        target_channel = server.create_channel(params[0], &client);
        client.send_message(params[0] + " created", client.get_fd());
        
    }
}