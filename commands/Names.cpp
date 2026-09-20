#include "Commands.hpp"

void Commands::Names(Client& client, const std::vector<std::string>& params, Server& server)
{
    std::string srv_name = server.get_server_name();
    std::string nick = client.get_nickname().empty() ? "*" : client.get_nickname();

    if (params.empty())
    {
        client.send_message(":" + srv_name + " 366 " + nick + " * :End of /NAMES list\r\n", client.get_fd());
        return;
    }
    std::string channel_name = params[0];
    Channel* channel = server.is_channel_exist(channel_name);
    if (!channel)
    {
        client.send_message(":" + srv_name + " 366 " + nick + " " + channel_name + " :End of /NAMES list\r\n", client.get_fd());
        return;
    }
    std::string nicks_list = "";
    std::map<int, Client*> members = channel->get_members();
    std::map<int, Client*>::iterator it;
    for (it = members.begin(); it != members.end(); ++it)
    {
        if (channel->is_operator(it->first))
            nicks_list += "@";
        nicks_list += it->second->get_nickname() + " ";
    }
    if (!nicks_list.empty())
        nicks_list.erase(nicks_list.length() - 1);
    client.send_message(":" + srv_name + " 353 " + nick + " = " + channel_name + " :" + nicks_list + "\r\n", client.get_fd());
    client.send_message(":" + srv_name + " 366 " + nick + " " + channel_name + " :End of /NAMES list\r\n", client.get_fd());
}