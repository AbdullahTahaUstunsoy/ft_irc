#include "../../inc/Commands.hpp"

void Commands::Ping(Client& client, const std::vector<std::string>& params)
{
    if (params.empty())
    {
        std::cout << "empty param for ping" << std::endl;
        return ;
    }
    std::string std = params[0];
    std::string stdmsg = "PONG " + std + "\r\n";
    client.send_message(stdmsg,client.get_fd());
}