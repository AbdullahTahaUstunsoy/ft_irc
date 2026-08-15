#include "Commands.hpp"

void Commands::Topic(Client& client, const std::vector<std::string>& params, Server& server)
{
    if (params.empty())
    {
        client.send_message("empty param", client.get_fd());
        return ;
    }
    if (server.is_channel_exist(params[0]))
    {
        Channel *target_channel = server.get_channel(params[0]);
        if (target_channel->is_mem(client.get_fd()))
        {
            if (params.size() == 1)
            {
                if (target_channel->get_topic().empty())
                {
                    client.send_message("no topic in this channel", client.get_fd());
                    return ;
                }
                client.send_message(server.get_channel(params[0])->get_topic(), client.get_fd());
                return ;
            }
            else if ( params.size() == 2)
            {
                // op mod will add
                target_channel->set_topic(params[1]);
                //broadcast message to announce new topic
                return ;
            }
        }
        client.send_message("you are not a member", client.get_fd());
        return ;
    }
    client.send_message("there no such a channel", client.get_fd());
    return ;
}