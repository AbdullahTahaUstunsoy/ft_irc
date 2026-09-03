#include "Commands.hpp"

int string_to_int(const std::string& str)
{
    if (str.empty())
        return -1;
    int limit = 0;
    for (size_t i = 0; i < str.length(); ++i)
    {
        if (!std::isdigit(str[i]))
            return -1;
            
        limit = (limit * 10) + (str[i] - '0');
    }
    if (limit <= 0)
        return -1;
    return limit;
}

void Commands::Mode(Client& client, const std::vector<std::string>& params, Server& server)
{
    std::string srv_name = server.get_server_name();
    std::string nick = client.get_nickname().empty() ? "*" : client.get_nickname();

    if(params.size() < 2)
    {
        // we have to check what does /mode #channel
        client.send_message(":" + srv_name + " 461 " + nick + " MODE :Not enough parameters", client.get_fd());
        return ;
    }
    Channel* target_channel = server.is_channel_exist(params[0]);
    if(!target_channel)
    {
        if (params[0] == nick)
        {
            if(params[1] == "+i" || params[1] == "-i")
            {
                client.send_message(":" + nick + " MODE " + nick + " :" + params[1], client.get_fd());
                return ;
            }
            else
            {
                client.send_message(":" + srv_name + " 403 " + nick + " " + params[0] + " :Wrong parameters", client.get_fd());
                return ;
            }

        }
        client.send_message(":" + srv_name + " 403 " + nick + " " + params[0] + " :No such channel", client.get_fd());
        return ;
    }
    if (!target_channel->is_member(client.get_fd()) || !target_channel->is_operator(client.get_fd()))
    {
        client.send_message(":" + srv_name + " 482 " + nick + " " + params[0] + " :You're not channel operator", client.get_fd());
        return ;
    }
    if(params[1] == "+i" || params[1] == "-i")
    {
        if (params[1][0] == '-')
            target_channel->set_invite(0);    
        else
            target_channel->set_invite(1);
    }
    else if(params[1] == "+k" || params[1] == "-k")
    {
        if (params[1][0] == '+')
            target_channel->set_key(1, params[2]);    
        else
            target_channel->set_key(0, NULL);
    }
    else if(params[1] == "+t" || params[1] == "-t")
    {
        if (params[1][0] == '+')
            target_channel->set_topic_rest(1);
        else
            target_channel->set_topic_rest(0);
    }
    else if(params[1] == "+o" || params[1] == "-o")
    {
        int fd = server.get_client_fd(params[2]);
        if (fd == -1)
            return ;
        if (!target_channel->is_member(fd))
            return ;
        if (params[1][0] == '+')
            target_channel->add_operator(fd);
        else
            target_channel->remove_operator(fd);
    }
    else if(params[1] == "+l" || params[1] == "-l")
    {
        if (params[1][0] == '+')
        {
            int limit = string_to_int(params[2]);
            if (limit == -1)
                return ;
            target_channel->set_member_limit(limit);
        }
        else
            target_channel->set_member_limit(-1);
    }

}