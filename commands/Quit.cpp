#include "Commands.hpp"

void Commands::Quit(Client& client, const std::vector<std::string>& params, Server& server)
{
    server.quit_util(client.get_fd(), params[0]);
}