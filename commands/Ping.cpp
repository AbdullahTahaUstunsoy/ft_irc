#include "Commands.hpp"

void Commands::Ping(Client& client, const std::vector<std::string>& params, Server& server)
{
    std::string srv_name = server.get_server_name();
    std::string nick = client.get_nickname().empty() ? "*" : client.get_nickname();

    if (params.empty())
    {
        client.send_message(":" + srv_name + " 409 " + nick + " :No origin specified", client.get_fd());
        return ;
    }
    std::string std = params[0];
    std::string stdmsg = ":" + srv_name + " PONG " + srv_name + " :" + std;
    client.send_message(stdmsg, client.get_fd());
}