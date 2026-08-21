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
    if (params.empty())
    {
        client.send_message("empty param",client.get_fd());
        return ;
    }
    if (!client.get_pass())
    {
        client.send_message("pass false",client.get_fd());
        return ;
    }
    if (!isValidNickname(params[0]))
    {
        return ;
    }
    if (server.is_nick_unique(params[0]))
    {
        client.set_nick_name(params[0]);
        return ;
    }
    else
    {
        client.send_message("nick in use", client.get_fd());
        return ;
    }
}