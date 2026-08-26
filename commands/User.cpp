#include "Commands.hpp"

void Commands::User(Client& client, const std::vector<std::string>& params, Server& server)
{
    std::string srv_name = server.get_server_name();
    std::string nick = client.get_nickname().empty() ? "*" : client.get_nickname();

    if (params.size() != 4)
    {
        client.send_message(":" + srv_name + " 461 " + nick + " USER :Not enough parameters", client.get_fd());
        return ;
    }
    if (client.is_register())
    {
        client.send_message(":" + srv_name + " 462 " + nick + " :Unauthorized command (already registered)", client.get_fd());
        return ;
    }
    client.set_user_name(params[0]);
    if (client.get_pass() == true && !(client.get_nickname().empty()))
    {
        client.set_real_name(params[3]);
        client.set_register();
    }
    else
    {
        client.send_message(":" + srv_name + " 451 " + nick + " :You have not registered (PASS or NICK missing)", client.get_fd());
        std::cout << client.get_pass() << std::endl;
        std::cout << (client.get_nickname().empty()) << std::endl;
        return ;
    }
    client.send_message(":" + srv_name + " 001 " + nick + " :Welcome to the Internet Relay Network " + nick, client.get_fd());
}
