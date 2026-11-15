#include "Commands.hpp"

void Commands::Cap(Client& client, const std::vector<std::string>& params, Server& server)
{
    (void)server;
    if (!params.empty() && params[0] == "LS")
        client.send_message("CAP * LS :\r\n", client.get_fd());
}