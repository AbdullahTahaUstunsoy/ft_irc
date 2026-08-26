#include "Commands.hpp"

bool isValidNickname(const std::string& nick) {
    if (nick.length() > 9)
        return false;

    if (!std::isalpha(nick[0]))
        return false;

    std::string allowedSpecial = "[]\\`_^{|}";
    for (size_t i = 1; i < nick.length(); ++i) {
        if (!std::isalnum(nick[i]) && allowedSpecial.find(nick[i]) == std::string::npos && nick[i] != '-')
            return false;
    }
    return true;
}

void Commands::Nick(Client& client, const std::vector<std::string>& params, Server& server)
{
    std::string srv_name = server.get_server_name();
    std::string nick = client.get_nickname().empty() ? "*" : client.get_nickname();

    if (params.empty())
    {
        client.send_message(":" + srv_name + " 431 " + nick + " :No nickname given", client.get_fd());
        return ;
    }
    if (!client.get_pass())
    {
        client.send_message(":" + srv_name + " 464 " + nick + " :Password incorrect or not provided", client.get_fd());
        return ;
    }
    if (!isValidNickname(params[0]))
    {
        client.send_message(":" + srv_name + " 432 " + nick + " " + params[0] + " :Erroneous nickname", client.get_fd());
        return ;
    }
    if (server.is_nick_unique(params[0]))
    {
        std::string old_nick = client.get_nickname();
        if (!old_nick.empty())
        {
            std::string nick_msg = ":" + old_nick + " NICK :" + params[0];
            client.send_message(nick_msg, client.get_fd());
            // in this case have to use broadcast messages for users on channels 
        }
        client.set_nick_name(params[0]);
        return ;
    }
    else
    {
        client.send_message(":" + srv_name + " 433 " + nick + " " + params[0] + " :Nickname is already in use", client.get_fd());
        return ;
    }
}