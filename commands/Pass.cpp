#include "Commands.hpp"

void Commands::Pass(Client& client, const std::vector<std::string>& params, Server& server)
{
    std::string srv_name = server.get_server_name();
    std::string nick = client.get_nickname().empty() ? "*" : client.get_nickname();

    if (params.empty())
    {
        client.send_message(":" + srv_name + " 461 " + nick + " PASS :Not enough parameters", client.get_fd());
        return ;
    }
    if (client.is_register())
    {
        client.send_message(":" + srv_name + " 462 " + nick + " :Unauthorized command (already registered)", client.get_fd());
        return ;
    }
    if (params[0] != server.getPassword())
    {
        client.send_message(":" + srv_name + " 464 " + nick + " :Password incorrect", client.get_fd());
        return ;
    }
    client.set_pass();
}