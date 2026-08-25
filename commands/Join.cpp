#include "Commands.hpp"

void Commands::Join(Client& client, const std::vector<std::string>& params, Server& server)
{
    Channel* target_channel;
    if (params.empty())
    {
        std::cout << "Burada 1" << std::endl;
        client.send_message("Not enough parameters", client.get_fd());
        return ;
    }
    if (!client.is_register())
    {
        std::cout << "Burada 2" << std::endl;
        client.send_message("You have not registered", client.get_fd());
        return ;
    }
    if (params[0][0] != '#' && params[0][0] != '&')
    {
        std::cout << "Burada 3" << std::endl;
        client.send_message(params[0] + "No such channel", client.get_fd());
        return ;
    }
    target_channel = server.is_channel_exist(params[0]);
    if (target_channel)
    {
        if(target_channel->get_invite())
        {
            if(!target_channel->is_invited(client.get_fd()))
            {
                client.send_message(
                    ":server 437 " + client.get_nickname() + " " + params[0] +
                    " :Can not Join Channel without invite", client.get_fd()
                );
                std::cout << "Burada 4" << std::endl;
                return ;
            }
            if(target_channel->get_key())
            {
                std::cout << "Burada 5" << std::endl;
                if (target_channel->get_password() != params[1])
                {
                    std::cout << "Burada 6" << std::endl;
                    return ;
                }
            }
            if (target_channel->get_member_limit() != -1)
            {
                std::cout << "Burada 7" << std::endl;
                if (static_cast<size_t>(target_channel->get_member_limit()) <= target_channel->get_members().size())
                {
                    std::cout << "Burada 8" << std::endl;
                    return ;
                }
            } 
        }
        std::cout << "Burada 9" << std::endl;
        target_channel->add_member(&client);
    }
    else
    {
        std::cout << "Burada 10" << std::endl;
        target_channel = server.create_channel(params[0], &client);
        client.send_message(params[0] + " created", client.get_fd());
    }
}