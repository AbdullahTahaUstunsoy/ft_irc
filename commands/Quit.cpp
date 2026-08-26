#include "Commands.hpp"

void Commands::Quit(Client& client, const std::vector<std::string>& params, Server& server)
{
    std::string msg = "Client Quit";
    if (!params.empty() && !params[0].empty())
        msg = params[0];
    client.send_message("ERROR :Closing Link: (" + msg + ")", client.get_fd());
    server.quit_util(client, msg);
}