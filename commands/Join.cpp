#include "Commands.hpp"

void Commands::Join(Client& client, const std::vector<std::string>& params, Server& server, Channel& channel)
{
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
    if (server.is_channel_exist(params[0]))
    {
        channel.add_member(&client);
    }
    else
    {
        server.create_channel(params[0], &client);
    }
}